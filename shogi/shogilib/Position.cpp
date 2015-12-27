#include "Position.h"
#include "Move.h"


Position::Position()
{
	this->Init();
}


Position::~Position()
{
}

/**
*初期化
*/
void Position::Init()
{
	this->resetBoard();

	this->hashkey_.Init(this->side_to_move_, this->board_, this->hand_);
}

void Position::InitHashKey()
{
	this->hashkey_.Init(this->side_to_move_, this->board_, this->hand_);
}

/**
* boardの初期化
*/
void Position::resetBoard()
{
	this->side_to_move_ = BLACK;

	for (int i = 0; i < PIECE_TYPE_NB; i++)
	{
		this->hand_[BLACK][i] = 0;
		this->hand_[WHITE][i] = 0;
	}

	for (int i = 0; i < SQUARE_NB; i++)
	{
		this->board_[i] = Piece::NO_PIECE;
	}

	this->board_[SQ_91] = W_LANCE;
	this->board_[SQ_81] = W_KNIGHT;
	this->board_[SQ_71] = W_SILVER;
	this->board_[SQ_61] = W_GOLD;
	this->board_[SQ_51] = W_KING;
	this->board_[SQ_41] = W_GOLD;
	this->board_[SQ_31] = W_SILVER;
	this->board_[SQ_21] = W_KNIGHT;
	this->board_[SQ_11] = W_LANCE;

	this->board_[SQ_82] = W_ROOK;
	this->board_[SQ_22] = W_BISHOP;

	for (int sq = SQ_93; sq <= SQ_13; sq++)
	{
		this->board_[sq] = W_PAWN;
	}

	for (int sq = SQ_97; sq <= SQ_17; sq++)
	{
		this->board_[sq] = B_PAWN;
	}

	this->board_[SQ_28] = B_ROOK;
	this->board_[SQ_88] = B_BISHOP;


	this->board_[SQ_99] = B_LANCE;
	this->board_[SQ_89] = B_KNIGHT;
	this->board_[SQ_79] = B_SILVER;
	this->board_[SQ_69] = B_GOLD;
	this->board_[SQ_59] = B_KING;
	this->board_[SQ_49] = B_GOLD;
	this->board_[SQ_39] = B_SILVER;
	this->board_[SQ_29] = B_KNIGHT;
	this->board_[SQ_19] = B_LANCE;
}

/*--------------------------------------------------------------------*/
/**
* 香落ち
*/
void Position::SetHandicapKyo()
{
	this->resetBoard();
	this->side_to_move_ = WHITE;

	this->board_[SQ_11] = NO_PIECE;
	this->hashkey_.Init(this->side_to_move_, this->board_, this->hand_);
}

// 右香落ち
void Position::SetHandicapRightKyo()
{
	this->resetBoard();
	this->side_to_move_ = WHITE; // 上手から指す

	this->board_[SQ_91] = NO_PIECE;
	this->hashkey_.Init(this->side_to_move_, this->board_, this->hand_);
}

// 角落ち
void Position::SetHandicapKaku()
{
	this->resetBoard();
	this->side_to_move_ = WHITE; // 上手から指す

	this->board_[SQ_22] = NO_PIECE;
	this->hashkey_.Init(this->side_to_move_, this->board_, this->hand_);
}

// 飛車落ち
void Position::SetHandicapHisya()
{
	this->resetBoard();
	this->side_to_move_ = WHITE; // 上手から指す

	this->board_[SQ_82] = NO_PIECE;
	this->hashkey_.Init(this->side_to_move_, this->board_, this->hand_);
}

// 飛香落ち
 void Position::SetHandicapHiKyo()
{
	this->resetBoard();
	this->side_to_move_ = WHITE; // 上手から指す

	this->board_[SQ_11] = NO_PIECE;
	this->board_[SQ_82] = NO_PIECE;
	this->hashkey_.Init(this->side_to_move_, this->board_, this->hand_);
}

// 2枚落ち
 void Position::SetHandicap2()
{
	this->resetBoard();
	this->side_to_move_ = WHITE; // 上手から指す

	this->board_[SQ_22] = NO_PIECE;
	this->board_[SQ_82] = NO_PIECE;
	this->hashkey_.Init(this->side_to_move_, this->board_, this->hand_);
}

// 3枚落ち
 void Position::SetHandicap3()
{
	this->resetBoard();
	this->side_to_move_ = WHITE; // 上手から指す

	this->board_[SQ_22] = NO_PIECE;
	this->board_[SQ_82] = NO_PIECE;

	this->board_[SQ_91] = NO_PIECE; // 右香
	this->hashkey_.Init(this->side_to_move_, this->board_, this->hand_);
}

// 4枚落ち
 void Position::SetHandicap4()
{
	this->resetBoard();
	this->side_to_move_ = WHITE; // 上手から指す

	this->board_[SQ_22] = NO_PIECE; // 角
	this->board_[SQ_82] = NO_PIECE; // 飛車
	this->board_[SQ_11] = NO_PIECE; // 左香
	this->board_[SQ_91] = NO_PIECE; // 右香
	this->hashkey_.Init(this->side_to_move_, this->board_, this->hand_);
}

// 5枚落ち
 void Position::SetHandicap5()
{
	this->resetBoard();
	this->side_to_move_ = WHITE; // 上手から指す

	this->board_[SQ_22] = NO_PIECE; // 角
	this->board_[SQ_82] = NO_PIECE; // 飛車
	this->board_[SQ_11] = NO_PIECE; // 左香
	this->board_[SQ_81] = NO_PIECE; // 右桂
	this->board_[SQ_91] = NO_PIECE; // 左香
	this->hashkey_.Init(this->side_to_move_, this->board_, this->hand_);
}

// 左5枚落ち
 void Position::SetHandicapLeft5()
{
	this->resetBoard();
	this->side_to_move_ = WHITE; // 上手から指す

	this->board_[SQ_22] = NO_PIECE; // 角
	this->board_[SQ_82] = NO_PIECE; // 飛車
	this->board_[SQ_11] = NO_PIECE; // 左香
	this->board_[SQ_21] = NO_PIECE; // 左桂
	this->board_[SQ_91] = NO_PIECE; // 右香
	this->hashkey_.Init(this->side_to_move_, this->board_, this->hand_);
}

// 6枚落ち
 void Position::SetHandicap6()
{
	this->resetBoard();
	this->side_to_move_ = WHITE; // 上手から指す

	this->board_[SQ_22] = NO_PIECE; // 角
	this->board_[SQ_82] = NO_PIECE; // 飛車
	this->board_[SQ_11] = NO_PIECE; // 左香
	this->board_[SQ_21] = NO_PIECE; // 左桂
	this->board_[SQ_81] = NO_PIECE; // 右桂
	this->board_[SQ_91] = NO_PIECE; // 右香
	this->hashkey_.Init(this->side_to_move_, this->board_, this->hand_);
}

// 8枚落ち
 void Position::SetHandicap8()
{
	this->resetBoard();
	this->side_to_move_ = WHITE; // 上手から指す

	this->board_[SQ_22] = NO_PIECE; // 角
	this->board_[SQ_82] = NO_PIECE; // 飛車
	this->board_[SQ_11] = NO_PIECE; // 左香
	this->board_[SQ_21] = NO_PIECE; // 左桂
	this->board_[SQ_31] = NO_PIECE; // 銀
	this->board_[SQ_71] = NO_PIECE; // 銀
	this->board_[SQ_81] = NO_PIECE; // 右桂
	this->board_[SQ_91] = NO_PIECE; // 右香
	this->hashkey_.Init(this->side_to_move_, this->board_, this->hand_);
}

// 10枚落ち
 void Position::SetHandicap10()
{
	this->resetBoard();
	this->side_to_move_ = WHITE; // 上手から指す

	this->board_[SQ_22] = NO_PIECE; // 角
	this->board_[SQ_82] = NO_PIECE; // 飛車
	this->board_[SQ_11] = NO_PIECE; // 左香
	this->board_[SQ_21] = NO_PIECE; // 左桂
	this->board_[SQ_31] = NO_PIECE; // 銀
	this->board_[SQ_41] = NO_PIECE; // 金
	this->board_[SQ_61] = NO_PIECE; // 金
	this->board_[SQ_71] = NO_PIECE; // 銀
	this->board_[SQ_81] = NO_PIECE; // 右桂
	this->board_[SQ_91] = NO_PIECE; // 右香
	this->hashkey_.Init(this->side_to_move_, this->board_, this->hand_);
}

/*--------------------------------------------------------------------*/
/**
 *反転
 */
void Position::Reverse()
{
	for (int i = 0; i < PIECE_TYPE_NB; i++)
	{
		int num = this->hand_[BLACK][i];

		this->hand_[BLACK][i] = this->hand_[WHITE][i];
		this->hand_[WHITE][i] = num;
	}

	// 反転
	for (int i = 0, j = SQUARE_NB - 1; i <= (SQUARE_NB / 2); i++, j--)
	{
		Piece piece = this->board_[i];

		this->board_[i] = opp(this->board_[j]);
		this->board_[j] = opp(piece);
	}

	this->hashkey_.Init(this->side_to_move_, this->board_, this->hand_);
}

 /*--------------------------------------------------------------------*/
 /**
 *ボードクリア(CSA用
 */
void Position::BoardClear()
{
	for (int i = 0; i < SQUARE_NB; i++)
	{
		this->board_[i] = NO_PIECE;
	}
}

/*--------------------------------------------------------------------*/
/**
*駒移動
* ※合法手しか受け付けない
*/
bool Position::DoMove(const Move& move)
{
	ASSERT_MSG(move.side() == this->side_to_move_, "パラメータエラー");
	bool ret = true;

	if (move.move_type() == PASS)
	{
		// パス
	}
	else  if (move.move_type() & DROP_FLAG)
	{
		// 打つ手
		ASSERT_MSG(move.to() >= 0 && move.to() < SQUARE_NB, "パラメータエラー");
		ASSERT_MSG(this->hand_[move.side()][type_of(move.piece())] != 0, "パラメータエラー");
		ASSERT_MSG(this->board_[move.to()] == NO_PIECE, "パラメータエラー");

		this->hand_[move.side()][type_of(move.piece())] -= 1;
		this->board_[move.to()] = move.piece();
	}
	else
	{
		// 指す手
		ASSERT_MSG(move.move_type() & MOVE_FLAG, "パラメータエラー");
		ASSERT_MSG(move.to() >= 0 && move.to() < SQUARE_NB, "パラメータエラー");
		ASSERT_MSG(move.from() >= 0 && move.from() < SQUARE_NB, "パラメータエラー");
		ASSERT_MSG(this->board_[move.from()] != NO_PIECE, "パラメータエラー");
		ASSERT_MSG(this->board_[move.from()] == move.piece(), "パラメターエラー");
		ASSERT_MSG(this->board_[move.to()] == move.capture_piece(), "パラメータエラー");
		
		Piece piece = move.piece();
		if (move.move_type() & PROMOTION_FLAG)
		{
			piece = Piece(piece + PIECE_PROMOTION_OFS);
		}

		this->board_[move.from()] = NO_PIECE;
		this->board_[move.to()] = piece;

		if (move.capture_piece() != NO_PIECE)
		{
			this->hand_[move.side()][type_of(move.capture_piece())] += 1;
		}
	}

	if (!ret)
	{
		return false;
	}

	this->hashkey_.DoMove(move);
	this->side_to_move_ = opp(this->side_to_move_); // 手番変更

	return true;
}


/*--------------------------------------------------------------------*/
/**
*駒移動戻す
*/
bool Position::UndoMove(const Move& move)
{
	bool ret = true;
	
	ASSERT_MSG(move.to() >= 0 && move.to() < SQUARE_NB, "パラメータエラー");
	ASSERT_MSG(move.move_type() & MOVE_FLAG, "パラメータエラー");
	ASSERT_MSG(move.side() == opp(this->side_to_move_), "パラメータエラー");

	if (move.move_type() == PASS)
	{
		// パス
	}
	else if (move.move_type() & DROP_FLAG)
	{
		// 打つ手
		this->hand_[move.side()][type_of(move.piece())] += 1;
		this->board_[move.to()] = NO_PIECE;
	}
	else
	{
		// 指す手
		this->board_[move.from()] = move.piece();
		this->board_[move.to()] = move.capture_piece();

		this->hand_[move.side()][type_of(move.capture_piece())] -= 1;
	}


	this->hashkey_.UndoMove(move);
	this->side_to_move_ = opp(this->side_to_move_); // 手番変更

	return true;
}

/*--------------------------------------------------------------------*/
/**
*比較
*/
bool Position::Equals(const Position& pos) const
{
	// ハッシュキーで比較
	if (!this->hashkey().Equals(pos.hashkey()))
	{
		return false;
	}
	
	for (int i = 0; i < SQUARE_NB; i++)
	{
		if (this->board_[i] != pos.board_[i])
		{
			return  false;
		}
	}

	// 一応全部比較

	for (int i = 0; i < PIECE_TYPE_NB; i++)
	{
		if (this->hand_[BLACK][i] != pos.hand_[BLACK][i])
		{
			return false;
		}

		if (this->hand_[WHITE][i] != pos.hand_[WHITE][i])
		{
			return false;
		}
	}

	if (this->side_to_move_ != pos.side_to_move_)
	{
		return false;
	}

	return true;
}

/*-----------------------------------------------------------------------------*/
/**
 * @brief 指定した駒の検索
 * @param piece 探したい駒
 * @return Square::SQUARE_NBより小さい 見つかった位置
 *         
 */
/*-----------------------------------------------------------------------------*/
Square Position::SearchPiece(Piece piece) const
{
	int sq;

	for (sq = 0; sq < Square::SQUARE_NB; sq++)
	{
		if (this->board_[sq] == piece)
		{
			break;
		}
	}

	return Square(sq);
}