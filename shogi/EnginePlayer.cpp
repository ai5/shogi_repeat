#include "EnginePlayer.h"

#include <string>
#include <cassert>

#include "GameTimer.h"

#include "USIEngine.h"
#include "USIString.h"

#include "Logger.h"
#include "MoveCheck.h"
#include "Notation.h"
#include "PvInfo.h"
#include "Sfen.h"
#include "StringUtil.h"


EnginePlayer::EnginePlayer(Color color, EnginePlayerListener* lisnter)
	: color_(color)
	, lisnter_(lisnter)
{
	assert(lisnter);
}


EnginePlayer::~EnginePlayer()
{
	this->Terminate();
}

/*-----------------------------------------------------------------------------*/
/**
 * @brief 開始
 * @param 
 * @note  
 */
/*-----------------------------------------------------------------------------*/
bool EnginePlayer::Init(const std::string& filename)
{
	std::unique_lock<std::mutex> lock(this->mtx_);

	if (this->state_ != EnginePlayerState::NONE) return false;


	this->engine_ = std::make_unique<USIEngine>();

	if (!this->engine_->Initialize(filename))
	{
		// 起動失敗？
		this->engine_ = nullptr;
		return false;
	}


	this->th_ = std::make_unique<std::thread>(&EnginePlayer::receive_thread, this);

	this->state_ = EnginePlayerState::INITIALIZING;
	this->send_cmd("usi");

	this->timer_.SetTimeOut(&EnginePlayer::InitTimeout, this);
	this->timer_.Start(30000); // タイムアウトは３０秒

	return true;
}

/*-----------------------------------------------------------------------------*/
/**
* @brief 終了
* @note
*/
/*-----------------------------------------------------------------------------*/
void EnginePlayer::Terminate()
{
	this->Stop();

	{
		std::unique_lock<std::mutex> lock(this->mtx_);

		if (this->state_ == EnginePlayerState::NONE || this->engine_ == nullptr) return;

		this->state_ = EnginePlayerState::NONE;

		this->send_cmd("quit");

		this->engine_->Terminate();
	}

	this->th_->join();
}

/*-----------------------------------------------------------------------------*/
/**
* @brief オプション設定
* @note isreadyの送信
*/
/*-----------------------------------------------------------------------------*/
void EnginePlayer::SetOptions(const std::map<std::string, std::string>& opt_name_value)
{
	std::unique_lock<std::mutex> lock(this->mtx_);

	this->engine_->SetOptions(opt_name_value);

	for (auto& pair : opt_name_value)
	{
		if (this->engine_->HasOption(pair.first))
		{
			std::string opt = "option name " + pair.first + " value " + this->engine_->GetOptonValue(pair.first);

			this->send_cmd(opt);
		}
	}
}

/*-----------------------------------------------------------------------------*/
/**
* @brief オプション取得
* @note isreadyの送信
*/
/*-----------------------------------------------------------------------------*/
const USIOptions& EnginePlayer::GetOptions() const
{
	return this->engine_->options(); 
}


/*-----------------------------------------------------------------------------*/
/**
* @brief isreadyの送信
*/
/*-----------------------------------------------------------------------------*/
void EnginePlayer::Ready()
{
	std::unique_lock<std::mutex> lock(this->mtx_);
	if (this->state_ == EnginePlayerState::INISIALIZED)
	{
		this->state_ = EnginePlayerState::WAIT_READY;
		this->send_cmd("isready");
	}
}

/*-----------------------------------------------------------------------------*/
/**
* @brief GameStart
* @note
*/
/*-----------------------------------------------------------------------------*/
void EnginePlayer::GameStart()
{
	std::unique_lock<std::mutex> lock(this->mtx_);
	if (this->state_ == EnginePlayerState::IDLE)
	{
		this->send_cmd("usinewgame");
	}
	else
	{
		ASSERT_MSG(0, "STATEがIDLE以外は呼び出してはいけません");
	}
}

/*-----------------------------------------------------------------------------*/
/**
* @brief GameOver
* @note
*/
/*-----------------------------------------------------------------------------*/
void EnginePlayer::GameOver(Color color)
{
	this->Stop(); // 一応呼ぶ

	{
		std::unique_lock<std::mutex> lock(this->mtx_);

		std::string str;
		if (color == Color::NO_COLOR)
		{
			str = "draw";
		}
		else if (color == this->color_)
		{
			str = "win";
		}
		else
		{
			str = "lose";
		}

		this->send_cmd("gameover " + str);
	}
}


/*-----------------------------------------------------------------------------*/
/**
* @brief 思考開始
* @note
*/
/*-----------------------------------------------------------------------------*/
int EnginePlayer::Go(Notation& notation, const GameTimer& time_info)
{
	std::unique_lock<std::mutex> lock(this->mtx_);
	if (this->ponder_ != nullptr && this->state_ == EnginePlayerState::PONDER)
	{
		if (this->ponder_->Equals(notation.move_current()))
		{
			// ponder hit
			this->ponder_ = nullptr;
			this->state_ = EnginePlayerState::GO;
			this->send_cmd("ponderhit");
			return this->transactionCounter_;
		}
	}

	this->ponder_ = nullptr;

	// stateではじく？
	if (!(this->state_ == EnginePlayerState::IDLE
		|| this->state_ == EnginePlayerState::GO
		|| this->state_ == EnginePlayerState::PONDER
		|| this->state_ == EnginePlayerState::STOP
		|| this->state_ == EnginePlayerState::GO
		)) {
		
		return this->transactionCounter_;
	}

	const GameTime& game_time = (this->color_ == BLACK) ? time_info.BlackTime : time_info.WhiteTime;
	int byoyomi = game_time.Byoyomi;
	if (game_time.Byoyomi == 0 && game_time.Time == 0)
	{
		byoyomi = 10 * 1000; // 暫定１０秒
	}

	GoRequest req(time_info.BlackTime.RemainTime, time_info.WhiteTime.RemainTime, byoyomi);
	if (notation.output_initial_position() || notation.handicap() != Handicap::HIRATE)
	{
		req.Sfen = Sfen::PositionToString(notation.initial_position(), 1);
	}

	req.Moves = Sfen::MovesToString(notation);
	req.Pos = notation.position();

	req.transactionNo = ++this->transactionCounter_;

	if (this->state_ == EnginePlayerState::GO || this->state_ == EnginePlayerState::PONDER)
	{
		this->go_req_ = req;
		this->is_go_req_ = true;

		// 停止させる
		this->state_ = EnginePlayerState::STOP;
		this->send_cmd("stop");
	}
	else
	{
		this->is_go_req_ = false;
		this->state_ = EnginePlayerState::GO;

		this->ExecGoReeust(req);
	}

	return this->transactionCounter_;
}

/*-----------------------------------------------------------------------------*/
/**
* @brief go reqの実行
* @note
*/
/*-----------------------------------------------------------------------------*/
void EnginePlayer::ExecGoReeust(const GoRequest& req)
{
	std::string str;

	this->pv_info_.clear(); // pv 情報のクリア

	this->pos_ = req.Pos;

	str = "position ";
	str += (req.Sfen == "") ? "startpos" : "sfen " + req.Sfen;
	str += " moves " + req.Moves;

	this->send_cmd(str);

	if (req.ReqType == GoRequest::Type::NORMAL)
	{
		str = StringUtil::Format("go btime %d wtime %d byoyomi %d",
			req.Btime, req.Wtime, req.Byoyomi);
		this->send_cmd(str);
	}
	else if (req.ReqType == GoRequest::Type::PONDER)
	{
		this->send_cmd("go ponder");
	}
	else
	{
		// 他は未対応
		this->send_cmd("go infinite");
	}

	this->transactionNo_ = req.transactionNo;
}

/*-----------------------------------------------------------------------------*/
/**
* @brief 先読み
* @note
*/
/*-----------------------------------------------------------------------------*/
int EnginePlayer::Ponder(const Notation& notation)
{
	std::unique_lock<std::mutex> lock(this->mtx_);
	if (this->ponder_ == nullptr || !(this->ponder_->move_type() && MoveType::MOVE_FLAG))
	{
		// null or 指し手じゃない
		return this->transactionCounter_; // 現在の値を返しとく
	}

	// stateではじく？
	if (this->state_ != EnginePlayerState::IDLE ) 
	{
		return this->transactionCounter_;
	}

	GoRequest req(GoRequest::Type::PONDER);

	if (notation.output_initial_position() || notation.handicap() != Handicap::HIRATE)
	{
		req.Sfen = Sfen::PositionToString(notation.initial_position(), 1);
	}

	req.Moves = Sfen::MovesToString(notation) + " " + Sfen::MoveToString(*this->ponder_);
	req.Pos = notation.position();

	req.transactionNo = ++this->transactionCounter_;

	this->state_ = EnginePlayerState::PONDER;
	this->ExecGoReeust(req);

	return this->transactionCounter_;
}

/*-----------------------------------------------------------------------------*/
/**
* @brief 停止
* @note
*/
/*-----------------------------------------------------------------------------*/
void EnginePlayer::Stop()
{
	std::unique_lock<std::mutex> lock(this->mtx_);

	if (this->state_ == EnginePlayerState::GO || this->state_ == EnginePlayerState::PONDER)
	{
		this->is_go_req_ = false;
		this->ponder_ = nullptr;
		this->send_cmd("stop");
	}
}

/*-----------------------------------------------------------------------------*/
/**
* @brief エンジンへコマンド送信
* @note
*/
/*-----------------------------------------------------------------------------*/
void EnginePlayer::send_cmd(const std::string &cmd)
{
	Logger::WriteLine(this->time_str(Timer<int>::Now()) + (color_ == BLACK ? " B> " : " W> ") + cmd);

	this->engine_->WriteLine(cmd);
}

/*-----------------------------------------------------------------------------*/
/**
* @brief 受信スレッド
* @note
*/
/*-----------------------------------------------------------------------------*/
void EnginePlayer::receive_thread()
{

	while (!this->cancel_)
	{
		std::string str;

		STRING_QUEUE_ERR err = this->engine_->ReadLine(str, -1);

		Logger::WriteLine(this->time_str(Timer<int>::Now()) + (color_ == BLACK ? " <B " : " <W ") + str);

		if (err == STRING_QUEUE_ERR::TIMEOUT)
		{
			// タイムアウト処理
			continue;
		}

		if (err != STRING_QUEUE_ERR::OK)
		{
			// エラの場合は終了
			break;
		}

		if (!this->cancel_)
		{
			this->receive_command(str);
		}
	}
}

/*-----------------------------------------------------------------------------*/
/**
* @brief engine -> guiのコマンド処理
* @note
*/
/*-----------------------------------------------------------------------------*/
void EnginePlayer::receive_command(const std::string& str)
{

	switch (this->state_)
	{
	case	EnginePlayerState::NONE:
		// 何もしない
		break;
	case	EnginePlayerState::INITIALIZING:
		if (str == "usiok")
		{
			this->state_ = EnginePlayerState::INISIALIZED;
			this->lisnter_->notifyInisialized(this->color_);
		}
		else
		{
			this->parse_option(str);
		}
		break;
	case	EnginePlayerState::WAIT_READY:
		if (str == "readyok")
		{
			this->state_ = EnginePlayerState::IDLE;
			this->lisnter_->notifyReadyOk(this->color_);
			// 保留してあるコマンドの処理
			this->handleIdleState();
		}

		break;
	case	EnginePlayerState::IDLE:
		break;
	case	EnginePlayerState::GO:
	case	EnginePlayerState::PONDER:
		{
			USITokenizer tok(str);
			std::string cmd = tok.GetToken();

			if (cmd == "bestmove")
			{
				if (this->state_ == EnginePlayerState::GO)
				{
					// ponder中のbestmoveは無視
					this->parse_bestmove(str);
				}

				this->state_ = EnginePlayerState::IDLE;
				this->handleIdleState();
			}
			else if (cmd == "info")
			{
				this->parse_info(str);
			}
		}
		break;
	case	EnginePlayerState::STOP:
		{
			USITokenizer tok(str);
			if (tok.GetToken() == "bestmove")
			{
				// bestmoveの場合
				this->state_ = EnginePlayerState::IDLE;
				// 保留してあるコマンドの処理
				this->handleIdleState();
			}
		}
		break;
	case	EnginePlayerState::TERMINATING:
		break;
	}

}

/*-----------------------------------------------------------------------------*/
/**
* @brief OPTIONコマンド処理
* @note
*/
/*-----------------------------------------------------------------------------*/
void EnginePlayer::parse_option(const std::string& str)
{
	USITokenizer tok(str);

	std::string cmd = tok.GetToken();

	if (cmd == "id")
	{
		// idの場合

		std::string name =  tok.GetToken();
		if (name == "name")
		{
			this->name_ = tok.GetTokenLast();
		}
		else if (name == "author")
		{
			this->author_ = tok.GetTokenLast();
		}
	}
	else if (cmd == "option")
	{
		this->engine_->AddOption(str);
	}
}

/*-----------------------------------------------------------------------------*/
/**
* @brief bestmoveの処理
* @note
*/
/*-----------------------------------------------------------------------------*/
void EnginePlayer::parse_bestmove(const std::string& str)
{
	USITokenizer tok(str);

	if (tok.GetToken() == "bestmove")
	{
		std::string move_str = tok.GetToken();
		std::string ponder = "";

		if (tok.GetToken() == "ponder")
		{
			ponder = tok.GetToken();
		}

		Move move = Sfen::ParseMove(this->pos_, move_str);
		if (ponder == "" || ponder == "(none)")
		{
			this->ponder_ = std::make_unique<Move>();
		}
		else
		{
			this->ponder_ = std::make_unique<Move>(Sfen::ParseMove(this->pos_, ponder));
		}

		this->lisnter_->notifyBestMove(this->color_, this->transactionNo_, move, *this->ponder_);
	}
}

/*-----------------------------------------------------------------------------*/
/**
* @brief bestmoveの処理
* @note
*/
/*-----------------------------------------------------------------------------*/
void EnginePlayer::parse_info(const std::string& str)
{
	USITokenizer tok(str);

	if (tok.GetToken() == "info")
	{
		std::string token;
		int time = 0;
		long long nodes = 0;
		int depth = 0;
		int seldepth = 0;
		int score = 0;
		int multipv = 0;
		int nps = 0;
		int mate = PvInfo::MateNone;
		bool have_pv = false;
		Moves moves;

		while ((token = tok.GetToken()) != "")
		{
			if (token == "depth")
			{
				StringUtil::ParseNum(tok.GetToken(), depth);
			}
			else if (token == "seldepth")
			{
				StringUtil::ParseNum(tok.GetToken(), seldepth);
			}
			else if (token == "time")
			{
				StringUtil::ParseNum(tok.GetToken(), time);
			}
			else if (token == "nodes")
			{
				StringUtil::ParseNum(tok.GetToken(), nodes);
			}
			else if (token == "pv")
			{
				have_pv = true;
				moves.clear();
				
				std::string value;
				Position pos = this->pos_;

				while ((value = tok.GetToken()) != "")
				{
					Move move = Sfen::ParseMove(pos, value);
					if (move.IsNoMove())
					{
						// 指し手じゃない？
						tok.Back(value); // 本来なら最後までPVのはずだが、そうじゃないエンジンが存在する
						break;
					}

					if (!MoveCheck::IsValid(pos, move))
					{
						break;
					}

					moves.push_back(move);
					pos.DoMove(move);
				}
			}
			else if (token == "score")
			{
				have_pv = true;

				token = tok.GetToken();
				if (token == "cp")
				{
					StringUtil::ParseNum(tok.GetToken(), score);
					if (this->color_ == WHITE) score = -score; // 後手はスコアを逆にする
				}
				else if (token == "mate")
				{
					std::string value = tok.GetToken();
					if (value[0] == '+')
					{
						mate = PvInfo::MateBlack;
						value = value.substr(1);
					}
					else if (value[0] == '-')
					{
						mate = PvInfo::MateWhite;
						value = value.substr(1);
					}

					if (value != "")
					{
						if (StringUtil::ParseNum(value, score))
						{
							if (mate == PvInfo::MateNone)
							{
								if (score <= 0)
								{
									mate = PvInfo::MateWhite;
								}
								else
								{
									mate = PvInfo::MateBlack;
								}
							}
						}
					}

					if (this->color_ == WHITE)
					{
						mate = -mate;
					}
				}
			}
			else if (token == "lowerbound")
			{
				// 本来はsocore cp 100 lowerbound みたいな形式でくるけどこれ単独で判別する
			}
			else if (token == "upperbound")
			{
			}
			else if (token == "currmove")
			{
				Move move =	Sfen::ParseMove(this->pos_, tok.GetToken());
				// 未実装
			}
			else if (token == "hashfull")
			{
			}
			else if (token == "nps")
			{
				StringUtil::ParseNum(tok.GetToken(), nps);
			}
			else if (token == "multipv")
			{
				StringUtil::ParseNum(tok.GetToken(), multipv);
			}
			else if (token == "string")
			{
				std::string value = tok.GetTokenLast();
				if (value.size() != 0)
				{
					// 未実装
				}

				break;
			}

		}

		if (have_pv)
		{
			this->pv_info_[multipv] = PvInfo( multipv, time, score, mate, nodes, depth, seldepth, moves);

			// なんか色々パースしてコールバックを呼ぶ
			this->lisnter_->notifyInfo(this->color_, this->transactionNo_, this->pv_info_[multipv]);
		}
	}
}

/*-----------------------------------------------------------------------------*/
/**
* @brief IDLEに遷移した時の処理
* @note
*/
/*-----------------------------------------------------------------------------*/
void EnginePlayer::handleIdleState()
{
	std::unique_lock<std::mutex> lock(this->mtx_);

	// 保留されてるコマンドの実行

	if (this->is_go_req_)
	{
		this->is_go_req_ = false;
		this->state_ = EnginePlayerState::GO;
		this->ExecGoReeust(this->go_req_);
	}

}

/*-----------------------------------------------------------------------------*/
/**
* @brief 
* @note
*/
/*-----------------------------------------------------------------------------*/
std::string EnginePlayer::time_str(TimePoint tp)
{
	int hour = static_cast<int>(tp / (60 * 60 * 1000));
	int time = tp % (60 * 60 * 1000);

	int min = (time / (60 * 1000));
	time = tp % (60 * 1000);
	int sec = time / 1000;
	int ms = time % 1000;

	return StringUtil::Format("%d:%02d:%02d.%d", hour, min, sec, ms / 100);
}


/*-----------------------------------------------------------------------------*/
/**
 * @brief 初期化のタイムアウト
 * @note  
 */
/*-----------------------------------------------------------------------------*/
void EnginePlayer::InitTimeout()
{
	if (this->state_ == EnginePlayerState::INITIALIZING)
	{
		Logger::WriteLine("initialize timeout");
		// 初期化中である
		this->lisnter_->notifyError(this->color_, -1, "初期化タイムアウト");
	}
}