
#include <iterator> 

#include "Notation.h"
#include "MovesEnumerator.h"

Notation::Notation()
{
	this->Init();
}

Notation::Notation(const Notation& notation) 
{
	this->position_ = notation.position_;
	this->initial_position_ = notation.initial_position_;
	this->black_name_ = notation.black_name_;
	this->white_name_ = notation.white_name_;
	this->handicap_ = notation.handicap_;
	this->output_initial_position_ = this->output_initial_position_;
	this->kifu_infos_ = notation.kifu_infos_;

	this->result_ = notation.result_;
	this->winner_ = notation.winner_;

	this->moves_ = notation.moves_;

	this->current_moves_ = &this->moves_;
	this->move_current_ = &this->moves_[0];

	this->Jump(notation.number());
}



Notation::~Notation()
{
}


/*-----------------------------------------------------------------------------*/
/**
* @brief コピー
* @note
*/
/*-----------------------------------------------------------------------------*/
Notation& Notation::operator=(const Notation& notation)
{
	this->position_ = notation.position_;
	this->initial_position_ = notation.initial_position_;
	this->black_name_ = notation.black_name_;
	this->white_name_ = notation.white_name_;
	this->handicap_ = notation.handicap_;
	this->output_initial_position_ = this->output_initial_position_;
	this->kifu_infos_ = notation.kifu_infos_;

	this->result_ = notation.result_;
	this->winner_ = notation.winner_;

	this->moves_ = notation.moves_;

	this->current_moves_ = &this->moves_;
	this->move_current_ = &this->moves_[0];

	this->Jump(notation.number());

	return *this;
}


/*--------------------------------------------------------------------*/
/**
*初期化
*/
void Notation::Init()
{
	this->position_.Init();
	this->initial_position_.Init();

	this->black_name_ = L"";
	this->white_name_ = L"";
	this->handicap_ = Handicap::HIRATE;
	this->output_initial_position_ = false;

	this->kifu_infos_.clear();

	this->moves_.clear();
	this->moves_.emplace_back(); // 初期局面を入れておく
	this->current_moves_ = &this->moves_;
	this->move_current_ = &this->moves_[0];
}

/**
*positionのhashkey初期化
*/
void Notation::InitHashKey()
{
	this->initial_position_.InitHashKey();
}

/*
*初期ポジションの設定
*/
void Notation::SetInitialPosition(const Position& pos)
{
	this->initial_position_ = pos;

	this->DecisionHandicap(); // ハンディキャップ設定
	this->initial_position_.InitHashKey();

	this->position_ = this->initial_position_;
}

/*
*手合割の判定
*/
void Notation::DecisionHandicap()
{
	Position pos;

	this->output_initial_position_ = false; // 一旦FALSE
	if (pos.Equals(this->initial_position_))
	{
		this->handicap_ = Handicap::HIRATE;
		return;
	}

	pos.SetHandicapKyo();
	if (pos.Equals(this->initial_position_))
	{
		this->handicap_ = Handicap::KYO;
		return;
	}

	pos.SetHandicapRightKyo();
	if (pos.Equals(this->initial_position_))
	{
		this->handicap_ = Handicap::RIGHT_KYO;
		return;
	}

	pos.SetHandicapKaku();
	if (pos.Equals(this->initial_position_))
	{
		this->handicap_ = Handicap::KAKU;
		return;
	}

	pos.SetHandicapHisya();
	if (pos.Equals(this->initial_position_))
	{
		this->handicap_ = Handicap::HISYA;
		return;
	}

	pos.SetHandicapHiKyo();
	if (pos.Equals(this->initial_position_))
	{
		this->handicap_ = Handicap::HIKYO;
		return;
	}

	pos.SetHandicap2();
	if (pos.Equals(this->initial_position_))
	{
		this->handicap_ = Handicap::H2;
		return;
	}

	pos.SetHandicap3();
	if (pos.Equals(this->initial_position_))
	{
		this->handicap_ = Handicap::H3;
		return;
	}

	pos.SetHandicap4();
	if (pos.Equals(this->initial_position_))
	{
		this->handicap_ = Handicap::H4;
		return;
	}

	pos.SetHandicap5();
	if (pos.Equals(this->initial_position_))
	{
		this->handicap_ = Handicap::H5;
		return;
	}

	pos.SetHandicapLeft5();
	if (pos.Equals(this->initial_position_))
	{
		this->handicap_ = Handicap::LEFT5;
		return;
	}

	pos.SetHandicap6();
	if (pos.Equals(this->initial_position_))
	{
		this->handicap_ = Handicap::H6;
		return;
	}

	pos.SetHandicap8();
	if (pos.Equals(this->initial_position_))
	{
		this->handicap_ = Handicap::H8;
		return;
	}

	pos.SetHandicap10();
	if (pos.Equals(this->initial_position_))
	{
		this->handicap_ = Handicap::H10;
		return;
	}

	this->output_initial_position_ = true;
	this->handicap_ = Handicap::OTHER;

}

/**
*ハンディキャップ設定
*/
void Notation::set_handicap(Handicap handicap)
{
	this->handicap_ = handicap;
	this->output_initial_position_ = false; // 一旦FALSE;

	switch (this->handicap_)
	{
	case Handicap::HIRATE:
		break;
	case Handicap::RIGHT_KYO:
		this->initial_position_.SetHandicapRightKyo();
		break;
	case Handicap::KYO:
		this->initial_position_.SetHandicapKyo();
		break;
	case Handicap::KAKU:
		this->initial_position_.SetHandicapKaku();
		break;
	case Handicap::HISYA:
		this->initial_position_.SetHandicapHisya();
		break;
	case Handicap::HIKYO:
		this->initial_position_.SetHandicapHiKyo();
		break;
	case Handicap::H2:
		this->initial_position_.SetHandicap2();
		break;
	case Handicap::H3:
		this->initial_position_.SetHandicap3();
		break;
	case Handicap::H4:
		this->initial_position_.SetHandicap4();
		break;
	case Handicap::H5:
		this->initial_position_.SetHandicap5();
		break;
	case Handicap::LEFT5:
		this->initial_position_.SetHandicapLeft5();
		break;
	case Handicap::H6:
		this->initial_position_.SetHandicap6();
		break;
	case Handicap::H8:
		this->initial_position_.SetHandicap8();
		break;
	case Handicap::H10:
		this->initial_position_.SetHandicap10();
		break;
	case Handicap::OTHER:
		// 何もしない
		this->output_initial_position_ = true;
		this->initial_position_.InitHashKey();
		break;
	default:
		ASSERT_MSG(false, "引数エラー");  // 知らない娘ですね
		break;
	}
}

/*--------------------------------------------------------------------*/
/**
 *指定指し手へ移動
 */
void Notation::Jump(int num)
{
	this->position_ = this->initial_position_;
	this->current_moves_ = &this->moves_;

	Moves::iterator ite = this->current_moves_->begin();

	while (ite != this->current_moves_->end())
	{
		MoveKif& move = *ite++;
		this->move_current_ = &move;
		
		if (is_move(move.move_type()))
		{
			// 指し手の場合局面をすすめる
			this->position_.DoMove(move);
		}

		if (move.number() == num)
		{
			// 所定の位置まできたので終了
			break;
		}

		if (move.getCurrentBranchNumber() >= 0)
		{
			// 分岐側
			this->current_moves_ = move.getCurrentBranch();
			ite = this->current_moves_->begin();
		}
	}
}

/**
*先頭へ移動
*/
void Notation::First()
{
	this->position_ = this->initial_position_;
	this->current_moves_ = &this->moves_;
	this->move_current_ = &this->moves_[0];
}

/**
*末尾へ移動
*/
void Notation::Last()
{
	int pos;

	pos = this->number() - (*this->current_moves_)[0].number();
	ASSERT_MSG(pos >= 0, "内部状態が変?");

	while (pos < (int)this->current_moves_->size())
	{
		if (this->move_current_->getCurrentBranchNumber() == -1)
		{
			pos++;

			if (pos >= (int)this->current_moves_->size())
			{
				break;
			}
		}
		else
		{
			this->current_moves_ = this->move_current_->getCurrentBranch();
			pos = 0;
		}

		this->move_current_ = &(*this->current_moves_)[pos];

		if (is_move(this->move_current_->move_type()))
		{
			// 指し手の場合局面をすすめる
			this->position_.DoMove(*this->move_current_);
		}
	}

}

/**
*指定した手数戻る
*/
bool Notation::Prev(int num)
{
	int pos;
	bool ret = false;

	pos = this->number() - (*this->current_moves_)[0].number();
	ASSERT_MSG(pos >= 0, "内部状態が変?");

	for (int i = 0; i < num; i++)
	{
		MoveKif& move = (*this->current_moves_)[pos];

		if (pos <= 0)
		{
			if (this->current_moves_->parent() == nullptr)
			{
				break;
			}

			// 親の分岐を取得
			this->current_moves_ = this->current_moves_->parent();

			pos = move.getParentNumber() - (*this->current_moves_)[0].number();
		}
		else
		{
			pos--;
		}

		if (is_move(move.move_type()))
		{
			// 指し手の場合局面をすすめる
			this->position_.UndoMove(move);
		}

		this->move_current_ = &(*this->current_moves_)[pos];

		ret = true; // 指し手の移動があったのでtrue;
	}

	return ret;
}

/**
*1手前の指し手を取得
*/
const MoveKif*  Notation::getPrevMove() const 
{
	int pos;
	bool ret = false;

	pos = this->number() - (*this->current_moves_)[0].number();
	ASSERT_MSG(pos >= 0, "内部状態が変?");

	MoveKif& move = (*this->current_moves_)[pos];

	pos--;

	Moves* moves = this->current_moves_;

	if (pos < 0 && moves->parent() != nullptr)
	{
		// 親の分岐を取得
		moves = moves->parent();

		pos = move.getParentNumber() - (*moves)[0].number();
	}

	if (pos < 0)
	{
		return nullptr;
	}

	return &(*moves)[pos];
}

/*-----------------------------------------------------------------------------*/
/**
* @brief 次の手を取得
* @param int num
*/
/*-----------------------------------------------------------------------------*/
const MoveKif* Notation::getNext() const
{
	if (this->move_current_->getCurrentBranchNumber() == -1)
	{
		int pos = this->move_current_->number() - (*this->current_moves_)[0].number() + 1;

		if (pos >= (int)this->current_moves_->size())
		{
			return nullptr; // 終端
		}

		return &(*this->current_moves_)[pos];
	}
	
	
	return &(*this->move_current_->getCurrentBranch())[0];
}

/*-----------------------------------------------------------------------------*/
/**
 * @brief 指定した手数分すすめる
 * @param int num
 */
/*-----------------------------------------------------------------------------*/
bool Notation::Next(int num)
{
	int pos;
	bool ret = false;

	pos = this->number() - (*this->current_moves_)[0].number();
	ASSERT_MSG(pos >= 0, "内部状態が変?");

	for (int i = 0; i < num && pos < (int)this->current_moves_->size(); i++)
	{
		if (this->move_current_->getCurrentBranchNumber() == -1)
		{
			pos++;

			if (pos >= (int)this->current_moves_->size())
			{
				break;
			}
		}
		else
		{
			this->current_moves_ = this->move_current_->getCurrentBranch();
			pos = 0;
		}


		ret = true;
		this->move_current_ = &(*this->current_moves_)[pos];

		if (is_move(this->move_current_->move_type()))
		{
			// 指し手の場合局面をすすめる
			this->position_.DoMove(*this->move_current_);
		}
	}

	return ret;
}

/*-----------------------------------------------------------------------------*/
/**
 * @brief 指し手の追加
 * @param move 指し手
 * @return true 指し手を追加した
 *         false 追加失敗
 * @note  
 */
/*-----------------------------------------------------------------------------*/
bool Notation::AddMove(const MoveKif& move)
{
	const MoveKif* prev_move = this->getPrevMove();
	const MoveKif& move_current = this->current_moves_->back();

	if (move.number() != 0 && move.number() != (this->move_current_->number() + 1))
	{
		// 番号が違う
		return false;
	}


	if (move_current.number() != this->number())
	{
		// 指し手の末尾と番号が違う場合 ブランチとして作成する
		this->current_moves_ = this->move_current_->MakeBranch(this->current_moves_);
	}

	int number = this->move_current_->number();
	
	// 要素追加
	this->current_moves_->emplace_back(move);
	this->move_current_ = &this->current_moves_->back();

	MoveKif& move_kif = this->current_moves_->back();

	if (move_kif.number() == 0)
	{
		move_kif.set_number(number + 1);
	}

	move_kif.set_side(this->position_.side_to_move());

	if ((move_kif.move_type() & MOVE_FLAG))
	{
		if (move_kif.to() == move_current.to())
		{
			// 同ほげをたてる
			move_kif.set_move_type(MoveType(move_kif.move_type() | MoveType::SAME_FLAG));
		}

		Piece capture_piece = position_.GetPiece(move_kif.to());
		if (capture_piece != NO_PIECE)
		{
			// 捕獲する場合 captureフラグを建てる
			move_kif.set_capture_piece(capture_piece);
			move_kif.set_move_type(MoveType(move_kif.move_type() | MoveType::CAPTURE_FLAG));
		}
	}
	else if (is_result(move_kif.move_type()))
	{
		this->result_ = move_kif.move_type();
		this->winner_ = this->getWinPlayer(move_kif);
	}

	if (move_kif.totalTime == 0)
	{
		// トータル時間の計算
		move_kif.totalTime = ((prev_move != nullptr) ? prev_move->totalTime : 0) + move_kif.time;
	}

	if (is_move(move_kif.move_type()))
	{
		// 盤面をすすめる
		this->position_.DoMove(move_kif);
	}

	return true;
}

/*-----------------------------------------------------------------------------*/
/**
* @brief 指し手のマージ
* @param move 指し手
* @return true 指し手を追加した
*         false 追加失敗
* @note 同じ指し手があればそちらに指し手を移動、無ければ追加する
*/
/*-----------------------------------------------------------------------------*/
bool Notation::MergeMove(const MoveKif& move)
{

	MoveBranchEnumerator be(this->current_moves_, this->move_current_);

	const MoveKif* next_move;

	while ((next_move = be.Next()) != nullptr)
	{
		if (move.Equals(*next_move))
		{
			// 同じ場合
			this->move_current_->setCurrentBranchNumber(be.current_no()); // カレントを変更して
			this->Next(1); // １手すすめる

			return true;
		}
	}

	// 無かったら追加
	return this->AddMove(move);
}

/*-----------------------------------------------------------------------------*/
/**
 * @brief 入力取り消し
 * @note  
 */
/*-----------------------------------------------------------------------------*/
void Notation::Back()
{
	int pos = this->number() - (*this->current_moves_)[0].number();

	this->Prev(1); // 1手戻す
	if ((pos + 1) >= (int)this->current_moves_->size())
	{
		// おしりの手の場合
		this->current_moves_->erase(this->current_moves_->begin() + pos, this->current_moves_->end());
	}
}

/*-----------------------------------------------------------------------------*/
/**
 * @brief 対局の再開
 * @param del_stop true 中断を削除する
 * @note  カレント位置がResultなら１手戻す
 *        カレント位置が中断ならその指し手を削除する
 */
/*-----------------------------------------------------------------------------*/
void Notation::Continue(bool del_stop)
{
	if (is_result(this->move_current_->move_type()))
	{
		// 結果の場合
		Move* move = this->move_current_;
		
		this->Prev(1); // 1手戻す

		if (del_stop && move->move_type() == MoveType::STOP)
		{
			// ブランチ削除 ※結果にブランチがぶら下がっていることはないからこれでok
			this->DeleteBranch(-1);
		}
	}
}


/*-----------------------------------------------------------------------------*/
/**
 * @brief Resultから勝ったプレイヤーを求める
 * @param move Result
 * @return 勝ったプレイヤーの色 不明の場合はNO_COLORを返す
 * @note  
 */
/*-----------------------------------------------------------------------------*/
Color Notation::getWinPlayer(const Move& move) const
{
	Color player = NO_COLOR;

	switch (move.move_type())
	{
	case MoveType::RESIGN: // 投了
	case MoveType::TIMEOUT: // 時間切れ
	case MoveType::LOSE_FOUL: // 反則負け
		player = opp(move.side()); // 相手が勝ち
		break;
	case MoveType::STOP: // 中断
	case MoveType::REPETITION: // 千日手
	case MoveType::DRAW: // 持将棋
		break;
	case MoveType::WIN_FOUL: // 反則勝ち
		player = move.side(); // 自分が勝ち
		break;
	}

	return player;
}

/*-----------------------------------------------------------------------------*/
/**
 * @brief ブランチ追加
 * @note カレントの指し手にブランチを追加する
 */
/*-----------------------------------------------------------------------------*/
void Notation::AddBranch(const Moves& src_moves)
{
	Moves* dest_moves = this->current_moves_; // 一旦カレントで作成
	MoveKif* dest_move = this->move_current_;


	const MoveKif& last_move = dest_moves->back();

	if (&last_move == dest_move)
	{
		// 追加先が末尾の場合
		dest_moves->insert(dest_moves->end(), src_moves.begin(), src_moves.end());
	}
	else
	{
		// 追加先が途中の場合
		dest_move->AddBranch(dest_moves, src_moves);
	}
}

/*-----------------------------------------------------------------------------*/
/**
* @brief 棋譜のマージ
* @note 
*/
/*-----------------------------------------------------------------------------*/
void Notation::MergeNotation(const Notation& notation)
{
	
	// 未実装
}

/*-----------------------------------------------------------------------------*/
/**
* @brief ブランチ削除
*/
/*-----------------------------------------------------------------------------*/
void Notation::DeleteBranch(int no)
{
	MoveKif* move = this->move_current_; // 一旦カレントに対する操作にしておく
	Moves* moves = this->current_moves_;
	int num = this->number();

	if (no == -1)
	{
		int pos = num - (*moves)[0].number();

		// 次の要素から最後までを削除
		moves->erase(moves->begin() + pos + 1, moves->end());

		if (move->branches().size() != 0)
		{
			// ブランチをずらす
			moves->insert(moves->end(), move->branches()[0]->begin(), move->branches()[0]->end());

			move->DeleteBranch(0);
		}
	}
	else
	{
		move->DeleteBranch(no);
	}
}

/*-----------------------------------------------------------------------------*/
/**
* @brief ブランチ入れ替え
*/
/*-----------------------------------------------------------------------------*/
void Notation::SwapBranch(int left, int right)
{
	this->SwapBranch(this->move_current_, this->current_moves_, left, right);
}

void Notation::SwapBranch(MoveKif* move, Moves* moves, int left, int right)
{

	int num = this->number();

	if (left == right)
	{
		return;
	}

	if (left == -1 || right == -1)
	{
		// 親とこどもの入れ替え
		if (right == -1)
		{
			std::swap(left, right);
		}
		
		int pos = num - (*moves)[0].number();

		// 次の要素から最後までをコピーー＞消去
		std::shared_ptr<Moves>temp = std::make_shared<Moves>();

		std::copy(moves->begin() + pos + 1, moves->end(), std::back_inserter(*temp));

		moves->erase(moves->begin() + pos + 1, moves->end());

		// ブランチをコピー
		moves->insert(moves->end(), move->branches()[right]->begin(), move->branches()[right]->end());

		// 取っておいたやつ上書き
		move->SwapParent(right, temp);
	}
	else
	{
		move->SwapBranch(left, right);
	}
}

/*-----------------------------------------------------------------------------*/
/**
 * @brief　こどものカレント変更
 */
/*-----------------------------------------------------------------------------*/
void Notation::ChangeChildCurrent(int no)
{
	this->move_current_->setCurrentBranchNumber(no);
}

/*-----------------------------------------------------------------------------*/
/**
* @brief カレント以外を削除する
*/
/*-----------------------------------------------------------------------------*/
void Notation::DeleteNotCurrent()
{
	bool ret = false;
	int number = this->number();
	
	size_t i;

	for (i = 0; i <= (size_t)number && i < this->moves_.size(); i++)
	{
		int no = this->moves_[i].getCurrentBranchNumber();

		if (no != -1)
		{
			this->SwapBranch(&this->moves_[i], &this->moves_, -1, no); // 分岐先を親に変更
		}

		// 分岐を全て削除
		this->moves_[i].ClearBranch();
	}

	// カレント以降を削除
	if (i < this->moves_.size())
	{
		this->moves_.erase(this->moves_.begin() + i, this->moves_.end());
	}

	this->current_moves_ = &this->moves_;
	this->Jump(number);
}