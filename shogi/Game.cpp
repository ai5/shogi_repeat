#include "Game.h"

#include <algorithm>

#include "DateTime.h"

#include "KeyValue.h"
#include "Kif.h"
#include "StringUtil.h"
#include "USIEngine.h"

#include "MoveCheck.h"
#include "Logger.h"
#include "PvInfo.h"

/*-----------------------------------------------------------------------------*/
/**
 */
/*-----------------------------------------------------------------------------*/
Game::Game() : engine_listner_(&this->msgq_), timer_(&Game::timeout, this)
{
}


Game::~Game()
{

}

/*-----------------------------------------------------------------------------*/
/**
 * @brief 開始
 * @note  
 */
/*-----------------------------------------------------------------------------*/
void Game::Start(const GameParam& param)
{
	std::unique_lock<std::mutex> lock(this->mtx_);
	this->param_ = param;
	this->terminate_ = false;

	Logger::SetOutputFile(param.SaveLog);
	Logger::SetOutputStdout(param.DispLog);

	this->th_ = std::thread(&Game::main_loop, this);
}
/*-----------------------------------------------------------------------------*/
/**
* @brief 終了
* @note スレッドの中からは呼び出さないこと
*/
/*-----------------------------------------------------------------------------*/
void Game::Terminate()
{
	{
		std::unique_lock<std::mutex> lock(this->mtx_);
		if (!this->terminate_)
		{
			this->terminate_ = true;

			this->Cancel();
			this->black_player_->Terminate();
			this->white_player_->Terminate();
		}
	}

	this->th_.join();
}


/*-----------------------------------------------------------------------------*/
/**
* @brief 連続対局
* @note
*/
/*-----------------------------------------------------------------------------*/
void Game::main_loop()
{
	this->game_init();

	while (!this->is_continuous_game_end() && !this->IsCancel())
	{
		std::shared_ptr<Message> msg = this->msgq_.Receive();

		switch (msg->ID())
		{
		case MessageID::Post:
			this->EventPost( *(static_cast<MessagePost*>(msg.get())));
			break;
		case MessageID::Timeout:
			this->EventTimeout(*msg);
			break;
		case MessageID::Cancel:
			this->EventTimeout(*msg);
			break;
		case MessageID::Initialized:
			this->EventInitialized(*(static_cast<MessageInitialized*>(msg.get())));
			break;
		case MessageID::Ready:
			this->EventReady(*(static_cast<MessageReady*>(msg.get())));
			break;
		case MessageID::BestMove:
			this->EventBestMove(*(static_cast<MessageBestMove*>(msg.get())));
			break;
		case MessageID::Error:
			this->cancel_ = true;
			break;
		}
	}
	// 中断処理？

	this->game_term();

	if (!this->cancel_)
	{
		// キャンセル以外の終了のしかたがexitしか思いつかない
		exit(0);
	}
}

/*-----------------------------------------------------------------------------*/
/**
* @brief 対局
* @note
*/
/*-----------------------------------------------------------------------------*/
bool Game::is_continuous_game_end()
{
	return (this->param_.MaxPlays > 0) && (this->count_ >= this->param_.MaxPlays);
}

/*-----------------------------------------------------------------------------*/
/**
* @brief 初期化
* @note
*/
/*-----------------------------------------------------------------------------*/
void Game::game_init()
{
	Logger::Open(this->GetLogName(DateTime::Now()));
	
	this->black_player_ = std::make_unique<EnginePlayer>(BLACK, &this->engine_listner_);
	this->white_player_ = std::make_unique<EnginePlayer>(WHITE, &this->engine_listner_);

	this->black_player_->Init(this->param_.Black.FileName);
}

/*-----------------------------------------------------------------------------*/
/**
* @brief 対局開始
* @note
*/
/*-----------------------------------------------------------------------------*/
void Game::game_start()
{
	this->play_ = true;

	std::cout << "game start " << (this->count_ + 1) << " " 
		<< this->param_.Black.Name << " vs " 
		<< this->param_.White.Name << std::endl;

	if (this->param_.UseNotation)
	{
		this->notation_ = this->param_.notation;

		if (this->param_.Num >= 0)
		{
			this->notation_.Jump(this->param_.Num); // 指定した手に移動

		}
		else
		{
			this->notation_.Last();
		}

		this->notation_.DeleteNotCurrent(); // 初期からカレント位置までを残して削除

		// カレント位置Resultならそれも削除
		if (is_result(this->notation_.move_current().move_type()))
		{
			this->notation_.Back();
		}

		this->history_.Init(this->notation_);
	}
	else
	{
		this->notation_.Init(); // 初期化
		this->notation_.set_handicap(this->param_.handicap);

		this->history_.Clear();

	}

	DateTime date = DateTime::Now();

	Logger::Open(this->GetLogName(date));

	this->start_date_ = date; // 対局開始時間を覚えとく
	this->notation_.set_kifu_info(L"開始日時", date.ToWString());
	this->notation_.set_black_name(this->param_.Black.WName);
	this->notation_.set_white_name(this->param_.White.WName);

	this->timer_.SetTime(BLACK, this->param_.Black.time, this->param_.Black.byoyomi);
	this->timer_.SetTime(WHITE, this->param_.White.time, this->param_.White.byoyomi);

	this->black_player_->GameStart();
	this->white_player_->GameStart();

	this->current_player()->Go(this->notation_, this->timer_);
	this->timer_.Start(this->notation_.position().side_to_move());
}

/*-----------------------------------------------------------------------------*/
/**
* @brief 対局終了
* @note
*/
/*-----------------------------------------------------------------------------*/
void Game::game_end()
{
	this->play_ = false;
	std::cout << "game end " << std::endl;

	// game overを呼び出す
	this->black_player_->GameOver(this->notation_.winner());
	this->white_player_->GameOver(this->notation_.winner());

	// 棋譜の保存

	Kif kif;
	kif.Save(this->notation_, this->param_.KifPath + this->GetKifuFileName(this->start_date_, this->param_.Black.Name, this->param_.White.Name));
	

	// ログは次のファイルOpenか終了でクローズされる	 

	// 次の対局設定　設定の入れ替えなど
	this->count_++;
	switch (this->notation_.winner())
	{
	case BLACK: this->black_win_++; break;
	case WHITE: this->white_win_++; break;
	}

	this->print_log();

	if (this->param_.SwapPlayer)
	{
		std::swap(this->param_.Black, this->param_.White);
		std::swap(this->black_player_, this->white_player_);

		std::swap(this->black_win_, this->white_win_);
		
		this->swap_flag = !this->swap_flag;
	}

	if (!is_continuous_game_end())
	{
		this->game_start();
	}
}

/*-----------------------------------------------------------------------------*/
/**
* @brief エンジン終了
* @note
*/
/*-----------------------------------------------------------------------------*/
void Game::game_term()
{
	this->timer_.Stop();
	this->black_player_->Terminate();
	this->white_player_->Terminate();

	// ログ記録終了
	Logger::Close();
}

/*-----------------------------------------------------------------------------*/
/**
 * @brief 結果を表示
 */
/*-----------------------------------------------------------------------------*/

void Game::print_log()
{
	int total_count = this->black_win_ + this->white_win_;

	int black_cnt = this->black_win_;
	int white_cnt = this->white_win_;

	if (this->swap_flag)
	{
		black_cnt = this->white_win_;
		white_cnt = this->black_win_;
	}

	std::cout <<
		StringUtil::Format("%s %d.%02d(%d) %s %d.%02d(%d) 引き分け %d\n",
			(this->swap_flag) ? this->param_.White.Name.c_str() : this->param_.Black.Name.c_str(),
			(total_count == 0) ? 0 : black_cnt * 100 / total_count,
			(total_count == 0) ? 0 : black_cnt * 10000 / total_count % 100,
			black_cnt,
			(this->swap_flag) ? this->param_.Black.Name.c_str() : this->param_.White.Name.c_str(),
			(total_count == 0) ? 0 : white_cnt * 100 / total_count,
			(total_count == 0) ? 0 : white_cnt * 10000 / total_count % 100,
			white_cnt,
			this->count_ - total_count
			);

}

/*-----------------------------------------------------------------------------*/
/**
* @brief post
*/
/*-----------------------------------------------------------------------------*/
void Game::EventPost(MessagePost& msg)
{
}

/*-----------------------------------------------------------------------------*/
/**
* @brief キャンセル
*/
/*-----------------------------------------------------------------------------*/
void Game::EventCancel(Message& msg)
{
	// 対局中なら
	if (this->play_)
	{
		this->play_ = false;

		this->timer_.Stop();

		this->black_player_->Stop();
		this->white_player_->Stop();
		this->black_player_->GameOver(this->notation_.winner());
		this->white_player_->GameOver(this->notation_.winner());

		this->notation_.AddMove(MoveKif(MoveType::STOP)); // 棋譜に中断をいれる
	}
}

/*-----------------------------------------------------------------------------*/
/**
* @brief 初期化完了
*/
/*-----------------------------------------------------------------------------*/
void Game::EventInitialized(MessageInitialized& msg)
{
	if (msg.Player == BLACK)
	{
		// 先手初期化完了
		this->white_player_->Init(this->param_.White.FileName);
	}
	else if (msg.Player == WHITE)
	{
		// 名前が設定されていない場合はこのタイミングで一度だけ設定される

		if (this->param_.Black.Name == "")
		{
			this->param_.Black.Name = this->black_player_->Name();
			this->param_.Black.WName = StringUtil::ConvertWStringFromString(this->black_player_->Name());
		}

		if (this->param_.White.Name == "")
		{
			this->param_.White.Name = this->white_player_->Name();
			this->param_.White.WName = StringUtil::ConvertWStringFromString(this->white_player_->Name());
		}

		// 先後のオプション設定
		this->LoadOption(BLACK, this->param_.Black.SettingFileName);
		this->LoadOption(WHITE, this->param_.White.SettingFileName);

		this->black_player_->Ready();
	}
}

/*-----------------------------------------------------------------------------*/
/**
* @brief ready
*/
/*-----------------------------------------------------------------------------*/
void Game::EventReady(MessageReady& msg)
{
	if (msg.Player == BLACK)
	{
		this->white_player_->Ready();
	}
	else
	{
		this->game_start(); // 対局開始
	}
}

/*-----------------------------------------------------------------------------*/
/**
* @brief BestMove
*/
/*-----------------------------------------------------------------------------*/
void Game::EventBestMove(MessageBestMove& msg)
{
	if (is_result(msg.BestMove.move_type()))
	{
		int time = this->timer_.Stop(); // タイマー止める

		// resultの場合
		if (msg.BestMove.move_type() == MoveType::NYUGYOKU_KACHI)
		{
			// 入玉判定 今は無条件でスルーする
		}

		MoveKif move(msg.BestMove);
		move.time = time;

		this->notation_.AddMove(move);

		this->game_end();
		
	}
	else
	{
		int time = this->timer_.ChangeTurn();

		MoveKif move(msg.BestMove);
		move.time = time;

		bool valid = MoveCheck::IsValid(this->notation_.position(), msg.BestMove);

		this->notation_.AddMove(move);

		// 指し手のチェック
		if (!valid)
		{
			// 相手方が反則勝ち
			this->notation_.AddMove(MoveKif(MoveType::WIN_FOUL));
			this->game_end();
		}
		else
		{
			// 着手の場合
			this->history_.Add(this->notation_.position().hashkey(), MoveCheck::IsCheck(this->notation_.position()));

			if (this->history_.IsRepetitionCheck())
			{
				// 連続王手の千日手
				this->notation_.AddMove(MoveKif(MoveType::WIN_FOUL));
				this->game_end();
			}
			else if (this->history_.IsRepetitionCheckOpp())
			{
				// 連続王手の千日手 相手側が連続王手
				this->notation_.AddMove(MoveKif(MoveType::LOSE_FOUL));
				this->game_end();
			}
			else if (this->history_.IsRepetition())
			{
				// 千日手
				this->notation_.AddMove(MoveKif(MoveType::REPETITION));
				this->game_end();
			}
			else if (this->param_.MaxMoves > 0 && this->notation_.number() >= this->param_.MaxMoves)
			{
				// 手数で引き分け
				this->notation_.AddMove(MoveKif(MoveType::REPETITION));
				this->game_end();
			}
			else
			{
				this->current_player()->Go(this->notation_, this->timer_);
				this->opp_player()->Ponder(this->notation_);

				this->timer_.TakeTurn();

				this->timer_.Start(this->notation_.position().side_to_move());
			}
		}
	}

	if (opp(this->notation_.position().side_to_move()) == BLACK)
	{
		this->AddComment(this->black_player_->PvInfos());
	}
	else
	{
		this->AddComment(this->white_player_->PvInfos());
	}

}



/*-----------------------------------------------------------------------------*/
/**
* @brief タイムアウト
*/
/*-----------------------------------------------------------------------------*/
void Game::EventTimeout(Message& msg)
{
	// タイムアウト時の処理
	// 入れ違いになることがあるので、ここで判別
	if (this->play_)
	{
		Color turn = this->notation_.position().side_to_move();

		GameRemainTime remain = this->timer_.GetRemainTime(turn);

		if (remain.HaveTime == 0 && remain.HaveByoyomi == 0)
		{
			// 時間制限なし
		}
		else
		{
			if (remain.Time == 0 && remain.Byoyomi == 0)
			{
				// 両方０の場合

				int time = this->timer_.Stop(); // タイマーを停止

			
				MoveKif move(MoveType::TIMEOUT); // タイムアウト負けにする
				move.time = time;

				this->notation_.AddMove(move);

				this->game_end();
			}
		}
	}
}

/*-----------------------------------------------------------------------------*/
/**
* @brief オプション読み込み
* @note
*/
/*-----------------------------------------------------------------------------*/
void Game::LoadOption(Color color, const std::string& filename)
{
	KeyValue kv;

	if (filename == "") return;

	if (!kv.Load(filename))
	{
		std::cerr << "file open error : " << filename << std::endl;
	}

	if (color == BLACK)
	{
		this->black_player_->SetOptions(kv);
	}
	else
	{
		this->white_player_->SetOptions(kv);
	}

}


/*-----------------------------------------------------------------------------*/
/**
* @brief オプション保存
* @note 排他とかオブジェクトとか気にしないので注意
*/
/*-----------------------------------------------------------------------------*/
void Game::SaveOption(Color color, const std::string& filename)
{
	KeyValue kv;

	if (filename == "") return;
	std::cout << "save option " << filename << std::endl;

	const USIOptions& options = (color == BLACK) ? this->black_player_->GetOptions() : this->white_player_->GetOptions();

	for (auto& opt : options)
	{
		kv[opt.second->Name] = opt.second->ValueToString();
	}

	kv.Save(filename);
}

/*-----------------------------------------------------------------------------*/
/**
* @brief　ログファイル名の作成
* @note
*/
/*-----------------------------------------------------------------------------*/
std::string Game::GetLogName(DateTime date)
{
	tm local_tm = date.LocalTime();


	return StringUtil::Format("%04d%02d%02d_%02d%02d%02d.log", local_tm.tm_year, local_tm.tm_mon, local_tm.tm_mday, local_tm.tm_hour, local_tm.tm_min, local_tm.tm_sec);
}

/*-----------------------------------------------------------------------------*/
/**
* @brief　棋譜ファイル名の作成
* @note
*/
/*-----------------------------------------------------------------------------*/
std::string Game::GetKifuFileName(DateTime date, const std::string& black_name, const std::string& white_name)
{
	tm local_tm = date.LocalTime();


	return StringUtil::Format("%04d%02d%02d_%02d%02d%02d_%s_vs_%s.kif",
		local_tm.tm_year, local_tm.tm_mon, local_tm.tm_mday,
		local_tm.tm_hour, local_tm.tm_min, local_tm.tm_sec,
		black_name.c_str(), white_name.c_str()
		);
}

/*-----------------------------------------------------------------------------*/
/**
 * @brief PV情報をコメントに追加
 * @note  
 */
/*-----------------------------------------------------------------------------*/
void Game::AddComment(const std::map<int, PvInfo>& pvinfos)
{
	for (auto& info : pvinfos)
	{
		std::wstring str = L"*対局 " + info.second.ToString();

		this->notation_.AddComment(str);
	}
}


/*-----------------------------------------------------------------------------*/
/**
* @brief タイムアウトのコールバック
* @note
*/
/*-----------------------------------------------------------------------------*/
void Game::timeout()
{
	this->msgq_.Send(std::make_shared<Message>(MessageID::Timeout));
}


/*=================================================================================*/

/*-----------------------------------------------------------------------------*/
/**
* @brief 初期化完了
*/
/*-----------------------------------------------------------------------------*/
void GameEngineListner::notifyInisialized(Color color)
{
	this->msgq_->Send(std::make_shared<MessageInitialized>(color));
}

/*-----------------------------------------------------------------------------*/
/**
* @brief Ready
*/
/*-----------------------------------------------------------------------------*/
void GameEngineListner::notifyReadyOk(Color color)
{
	this->msgq_->Send(std::make_shared<MessageReady>(color));
}

/*-----------------------------------------------------------------------------*/
/**
* @brief Bestmove
*/
/*-----------------------------------------------------------------------------*/
void GameEngineListner::notifyBestMove(Color color, int transactionNo, const Move& bestmove, const Move& ponder)
{
	this->msgq_->Send(std::make_shared<MessageBestMove>(color, transactionNo, bestmove, ponder));
}

/*-----------------------------------------------------------------------------*/
/**
* @brief info
*/
/*-----------------------------------------------------------------------------*/
void GameEngineListner::notifyInfo(Color color, int transactionNo, const PvInfo& )
{
}

/*-----------------------------------------------------------------------------*/
/**
* @brief error
*/
/*-----------------------------------------------------------------------------*/
void GameEngineListner::notifyError(Color color, int errorNo, const std::string& errorMsg)
{
	this->msgq_->Send(std::make_shared<MessageError>(color, errorNo));
}
