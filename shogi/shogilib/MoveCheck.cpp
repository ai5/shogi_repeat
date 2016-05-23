
#include <cstring>

#include "MoveCheck.h"


#include "Position.h"


#define NUMBER_OF(x) (sizeof(x) / sizeof((x)[0]))


const MoveCoord MoveCheck::FuMoveOfs[] = 
{
	{ 0, -1}
};

const MoveCoord MoveCheck::KeiMoveOfs[] = {
	{ -1, -2 }, {1, -2},
};

const MoveCoord MoveCheck::GinMoveOfs[] =
{
	{ -1, -1 },{ 0, -1 },{ 1, -1 },
	{ -1,  1 },          { 1,  1 },
};

const MoveCoord MoveCheck::KinMoveOfs[] = 
{
	{ -1, -1 },{ 0, -1 },{ 1, -1 },
	{ -1,  0 },          { 1,  0 },
	           { 0,  1 },          
};

const MoveCoord MoveCheck::GyokuMoveOfs[] = 
{
	{ -1, -1 },{ 0, -1 },{ 1, -1 },
	{ -1,  0 },          { 1,  0 },
	{ -1,  1 },{ 0,  1 },{ 1,  1 },
};



/*-----------------------------------------------------------------------------*/
/**
 * @brief 移動の可否判定　指し手が以下の反則の場合にfalseを返す
 * @note 1. 動けないところに駒をすめる
 * @note 2. 二歩
 * @note 3. 打ち歩詰め
 * @note 4. 王手放置
 */
/*-----------------------------------------------------------------------------*/
bool MoveCheck::IsValid(const Position& pos, const Move& move)
{
	bool ret = true;

	if (move.move_type() == MoveType::PASS)
	{
		return true;
	}

	ret = MoveCheck::MoveCheckParam(pos, move);

	if (ret)
	{
		if (move.move_type() & DROP_FLAG)
		{
			ret = MoveCheck::MoveCheckDrop(pos, move);
		}
		else
		{
			ret = MoveCheck::MoveCheckMove(pos, move);
		}

		// 王手放置チェック
		if (ret)
		{
			ret = !MoveCheck::IsMateLeft(pos, move);
		}
	}

	return ret;
}

/*-----------------------------------------------------------------------------*/
/**
* @brief 王手チェック
*/
/*-----------------------------------------------------------------------------*/
bool MoveCheck::IsCheck(const Position& pos)
{
	bool ret = false;

	if (pos.side_to_move() == WHITE)
	{
		ret = IsMateLeft<WHITE>(pos);
	}
	else
	{
		ret = IsMateLeft<BLACK>(pos);
	}

	return ret;
}


/*-----------------------------------------------------------------------------*/
/**
* @brief パラメータチェック
*/
/*-----------------------------------------------------------------------------*/
bool MoveCheck::MoveCheckParam(const Position& pos, const Move& move)
{
	// moveの内容物確認

	if (!is_move(move.move_type()))
	{
		return false;
	}

	// ピースのチェック
	if (move.piece() == Piece::NO_PIECE)
	{
		return false;
	}

	// ターンが違う場合
	if (pos.side_to_move() != color_of(move.piece()) || pos.side_to_move() != move.side())
	{
		// Debug.Assert(false, "引数エラー");
		return false;
	}

	if (!in_board(move.to()))
	{
		return false;
	}

	if (move.move_type() & DROP_FLAG)
	{
		// 打ち

		//　余計なフラグが立っていない
		if (move.move_type() & (SAME_FLAG | PROMOTION_FLAG))
		{
			return false;
		}

		if (is_promoted(move.piece()))
		{
			return false;
		}
	}
	else
	{
		if (!in_board(move.from()))
		{
			return false;
		}

		if (move.move_type() & PROMOTION_FLAG)
		{
			if (!MoveCheck::CanPromota(move))
			{
				return false;
			}
		}
		else
		{
			if (MoveCheck::ForcePromotion(move.piece(), move.to()))
			{
				return false;
			}
		}
	}

	return true;
}

/*-----------------------------------------------------------------------------*/
/**
* @brief 打つ手のチェック
*/
/*-----------------------------------------------------------------------------*/
bool MoveCheck::MoveCheckDrop(const Position& pos, const Move& move)
{
	// 持ち駒チェック
	if (!pos.IsHand(pos.side_to_move(), type_of(move.piece())))
	{
		return false;
	}

	// 打つところの駒チェック
	if (pos.GetPiece(move.to()) != Piece::NO_PIECE)
	{
		// 何かしら駒がある場合
		return false;
	}

	switch (move.piece())
	{
	case B_PAWN:
	case B_LANCE:
		if (rank_of(move.to()) <= Rank::RANK_1)
		{
			// 1段目には打てません
			return false;
		}

		break;
	case W_PAWN:
	case W_LANCE:
		// 後手
		if (rank_of(move.to()) >= Rank::RANK_9)
		{
			// 9段目には打てません
			return false;
		}
		break;
	case B_KNIGHT:
		if (rank_of(move.to()) <= Rank::RANK_2)
		{
			// 1,2段目には打てません
			return false;
		}

		break;
	case W_KNIGHT:
		if (rank_of(move.to()) >= Rank::RANK_8)
		{
			// 8,9段目には打てません
			return false;
		}

		break;
	}

	if (move.piece() == Piece::B_PAWN || move.piece() == Piece::W_PAWN)
	{
		// 二歩チェック
		if (MoveCheck::PieceCountInFile(pos, move.piece(), file_of(move.to())) >= 1)
		{
			// 2歩やがな
			return false;
		}

		if (pos.side_to_move() == BLACK)
		{
			if (MoveCheck::IsPawnDropMate<BLACK> (pos, move))
			{
				// 打ち歩詰
				return false;
			}
		}
		else
		{
			if (MoveCheck::IsPawnDropMate<WHITE>(pos, move))
			{
				// 打ち歩詰
				return false;
			}
		}
	}


	return true;
}

/*-----------------------------------------------------------------------------*/
/**
* @brief 移動する手のチェック
*/
/*-----------------------------------------------------------------------------*/
bool MoveCheck::MoveCheckMove(const Position& pos, const Move& move)
{
	// 移動先のチェック
	Piece from_piece = pos.GetPiece(move.to());

	if (from_piece != Piece::NO_PIECE)
	{
		// 移動先に現在のターンのコマ＝＝自分の駒がある
		if (pos.side_to_move() == color_of(from_piece))
		{
			// 自分の駒のあるところには移動出来ません
			return false;
		}
	}

	bool move_check = false;
	
	int move_rank_ofs;
	int move_file_ofs;

	move_rank_ofs = rank_of(move.to()) - rank_of(move.from());
	move_file_ofs = file_of(move.to()) - file_of(move.from());

	switch (move.piece())
	{
	case Piece::B_PAWN: // 歩
		move_check = CanMove(FuMoveOfs, NUMBER_OF(FuMoveOfs), move_file_ofs, move_rank_ofs);
		break;
	case Piece::W_PAWN: // 歩
					// チェック用に符号を反転させる
		move_check = CanMove(FuMoveOfs, NUMBER_OF(FuMoveOfs), -move_file_ofs, -move_rank_ofs);
		break;
	case Piece::B_LANCE: // 香車
		if (move_rank_ofs >= 0)
		{
			// 逆方向には動けません
			move_check = false;
		}
		else
		{
			move_check = CanMoveRank(pos, move);
		}

		break;
	case Piece::W_LANCE: // 香車
		if (move_rank_ofs <= 0)
		{
			move_check = false;
		}
		else
		{
			move_check = CanMoveRank(pos, move);
		}

		break;
	case Piece::B_KNIGHT: // 桂馬
		move_check = CanMove(KeiMoveOfs, NUMBER_OF(KeiMoveOfs), move_file_ofs, move_rank_ofs);
		break;
	case Piece::W_KNIGHT: // 桂馬
		move_check = CanMove(KeiMoveOfs, NUMBER_OF(KeiMoveOfs), -move_file_ofs, -move_rank_ofs);
		break;
	case Piece::B_SILVER: // 銀
		move_check = CanMove(GinMoveOfs, NUMBER_OF(GinMoveOfs), move_file_ofs, move_rank_ofs);
		break;
	case Piece::W_SILVER: // 銀
		move_check = CanMove(GinMoveOfs, NUMBER_OF(GinMoveOfs), -move_file_ofs, -move_rank_ofs);
		break;
	case Piece::B_BISHOP: // 角
	case Piece::W_BISHOP: // 角
		move_check = CanMoveDiagonal(pos, move);
		break;
	case Piece::B_PRO_BISHOP: // 馬
	case Piece::W_PRO_BISHOP: // 馬
		if (pos.side_to_move() == Color::WHITE)
		{
			// チェック用に符号を反転させる
			move_check = CanMove(GyokuMoveOfs, NUMBER_OF(GyokuMoveOfs), -move_file_ofs, -move_rank_ofs);
		}
		else
		{
			move_check = CanMove(GyokuMoveOfs, NUMBER_OF(GyokuMoveOfs), move_file_ofs, move_rank_ofs);
		}

		if (!move_check)
		{
			move_check = CanMoveDiagonal(pos, move);
		}

		break;
	case Piece::B_ROOK: // 飛車
	case Piece::W_ROOK: // 飛車

		move_check = CanMoveRank(pos, move);
		if (!move_check)
		{
			move_check = CanMoveFile(pos, move);
		}

		break;
	case Piece::B_PRO_ROOK: // 龍
	case Piece::W_PRO_ROOK: // 龍
		if (pos.side_to_move() == Color::WHITE)
		{
			// チェック用に符号を反転させる
			move_check = CanMove(GyokuMoveOfs, NUMBER_OF(GyokuMoveOfs), -move_file_ofs, -move_rank_ofs);
		}
		else
		{
			move_check = CanMove(GyokuMoveOfs, NUMBER_OF(GyokuMoveOfs), move_file_ofs, move_rank_ofs);
		}

		if (!move_check)
		{
			move_check = CanMoveRank(pos, move);
			if (!move_check)
			{
				move_check = CanMoveFile(pos, move);
			}
		}

		break;
	case Piece::B_KING: // 玉
		move_check = CanMove(GyokuMoveOfs, NUMBER_OF(GyokuMoveOfs), move_file_ofs, move_rank_ofs);
		break;
	case Piece::W_KING: // 玉
					// チェック用に符号を反転させる
		move_check = CanMove(GyokuMoveOfs, NUMBER_OF(GyokuMoveOfs), -move_file_ofs, -move_rank_ofs);
		break;
	case Piece::B_GOLD:
	case Piece::W_GOLD:
	default:
		// 金の動きをする駒
		if (pos.side_to_move() == Color::WHITE)
		{
			// チェック用に符号を反転させる
			move_check = CanMove(KinMoveOfs, NUMBER_OF(KinMoveOfs), -move_file_ofs, -move_rank_ofs);
		}
		else
		{
			move_check = CanMove(KinMoveOfs, NUMBER_OF(KinMoveOfs), move_file_ofs, move_rank_ofs);
		}

		break;
	}

	if (!move_check)
	{
		return false;
	}

	return true;
}

/*-----------------------------------------------------------------------------*/
/**
* @brief 王手放置チェック
* @note
*/
/*-----------------------------------------------------------------------------*/
bool MoveCheck::IsMateLeft(const Position& pos, const Move& move)
{
	Position pos_next = pos;
	bool ret = false;

	if (pos_next.DoMove(move))
	{
		if (pos.side_to_move() == WHITE)
		{
			ret = IsMateLeft<WHITE>(pos_next);
		}
		else
		{
			ret = IsMateLeft<BLACK>(pos_next);
		}
	}

	return ret;
}

/*-----------------------------------------------------------------------------*/
/**
 * @brief 縦方向の指定駒カウント（2歩チェックに使います
 * @note  
 */
/*-----------------------------------------------------------------------------*/
int MoveCheck::PieceCountInFile(const Position& pos, Piece piece, File file)
{
	int cnt = 0;

	for (Rank rank = RANK_1; rank < RANK_NB; rank += 1)
	{
		if (pos.GetPiece(file, rank) == piece)
		{
			cnt++;
		}
	}

	return cnt;
}

/*-----------------------------------------------------------------------------*/
/**
* @brief 効きデータクリア
* @note
*/
/*-----------------------------------------------------------------------------*/
void MoveCheck::ClearEffectData(int effect_data[SQUARE_NB])
{
	memset(effect_data, 0, sizeof(int) * (int)SQUARE_NB);
}

/*-----------------------------------------------------------------------------*/
/**
* @brief 効き作成
* @note
*/
/*-----------------------------------------------------------------------------*/
template<Color Us>
void MoveCheck::MakeEffectAll(const Position& pos, int effect_data[SQUARE_NB], Piece skip_piece)
{
	Piece piece;

	for (Square sq = SQ_ZERO; sq < SQUARE_NB; sq += 1)
	{
		piece = pos.GetPiece(sq);
		if (color_of(piece) == Us && piece != skip_piece)
		{
			// 先手の駒がある
			// 利きデータの作成
			MakeEfectOne<Us>(pos, sq, effect_data);
		}
	}
}

/*-----------------------------------------------------------------------------*/
/**
 * @brief 先手一つの駒の効き作成
 * @param 
 * @note  
 */
/*-----------------------------------------------------------------------------*/

template<Color Us>
void MoveCheck::MakeEfectOne(const Position& pos, Square square, int effect_data[SQUARE_NB])
{
	Piece piece = pos.GetPiece(square);

	// Debug.Assert(piece.ColorOf() == Color::BLACK, "引数エラー");

	Rank rank = rank_of(square);
	File file = file_of(square);

	// 利きデータを作成
	switch (piece)
	{
	case Piece::B_PAWN: // 歩
	case Piece::W_PAWN: // 歩
		MakeEffect(Us, effect_data, FuMoveOfs, NUMBER_OF(FuMoveOfs), file, rank);
		break;
	case Piece::B_LANCE: // 香車
	case Piece::W_LANCE: // 香車
		MakeEffect(pos, effect_data, file, rank, 0, -1);

		break;
	case Piece::B_KNIGHT: // 桂馬
	case Piece::W_KNIGHT: // 桂馬
		MakeEffect(Us, effect_data, KeiMoveOfs, NUMBER_OF(KeiMoveOfs), file, rank);
		break;
	case Piece::B_SILVER: // 銀
	case Piece::W_SILVER: // 銀
		MakeEffect(Us, effect_data, GinMoveOfs, NUMBER_OF(GinMoveOfs), file, rank);

		break;
	case Piece::B_BISHOP: // 角
	case Piece::W_BISHOP: // 角
		MakeEffect(pos, effect_data, file, rank, -1, -1);
		MakeEffect(pos, effect_data, file, rank, 1, -1);
		MakeEffect(pos, effect_data, file, rank, -1, 1);
		MakeEffect(pos, effect_data, file, rank, 1, 1);

		break;
	case Piece::B_PRO_BISHOP: // 馬
	case Piece::W_PRO_BISHOP: // 馬
		MakeEffect(Us, effect_data, GyokuMoveOfs, NUMBER_OF(GyokuMoveOfs), file, rank);

		MakeEffect(pos, effect_data, file, rank, -1, -1);
		MakeEffect(pos, effect_data, file, rank, 1, -1);
		MakeEffect(pos, effect_data, file, rank, -1, 1);
		MakeEffect(pos, effect_data, file, rank, 1, 1);

		break;
	case Piece::B_ROOK: // 飛車
	case Piece::W_ROOK: // 飛車
		MakeEffect(pos, effect_data, file, rank, 0, -1);
		MakeEffect(pos, effect_data, file, rank, 0, 1);
		MakeEffect(pos, effect_data, file, rank, -1, 0);
		MakeEffect(pos, effect_data, file, rank, 1, 0);
		break;
	case Piece::B_PRO_ROOK: // 龍
	case Piece::W_PRO_ROOK: // 龍
		MakeEffect(Us, effect_data, GyokuMoveOfs, NUMBER_OF(GyokuMoveOfs), file, rank);
		MakeEffect(pos, effect_data, file, rank, 0, -1);
		MakeEffect(pos, effect_data, file, rank, 0, 1);
		MakeEffect(pos, effect_data, file, rank, -1, 0);
		MakeEffect(pos, effect_data, file, rank, 1, 0);
		break;
	case Piece::B_KING: // 玉
	case Piece::W_KING: // 玉
		MakeEffect(Us, effect_data, GyokuMoveOfs, NUMBER_OF(GyokuMoveOfs), file, rank);
		break;
	case Piece::B_GOLD:
	case Piece::W_GOLD:
	default:
		MakeEffect(Us, effect_data, KinMoveOfs, NUMBER_OF(KinMoveOfs), file, rank);

		break;
	}
}

/*-----------------------------------------------------------------------------*/
/**
 * @brief 効き作成
 */
/*-----------------------------------------------------------------------------*/
void MoveCheck::MakeEffect(Color side, int effect_data[SQUARE_NB], const MoveCoord move_ofs[], size_t num, File move_to_file, Rank move_to_rank)
{
	File file;
	Rank rank;

	for (size_t i = 0; i < num;i++ )
	{
		const MoveCoord& ofs = move_ofs[i];

		if (side == BLACK)
		{
			rank = Rank(move_to_rank + ofs.rank);
			file = File(move_to_file + ofs.file);
		}
		else
		{
			rank = Rank(move_to_rank - ofs.rank);
			file = File(move_to_file - ofs.file);
		}

		if (Position::InBoard(file, rank))
		{
			effect_data[ make_square(file, rank)]++; // 利きがありまっせ
		}
	}
}

/*-----------------------------------------------------------------------------*/
/**
* @brief 飛車角、香車の利きデータ作成
*/
/*-----------------------------------------------------------------------------*/
void MoveCheck::MakeEffect(const Position& pos, int effect_data[SQUARE_NB], File file, Rank rank, int file_ofs, int rank_ofs)
{
	for (int i = 0; i < RANK_NB; i++)
	{
		rank += rank_ofs;
		file += file_ofs;

		if (!Position::InBoard(file, rank))
		{
			break;
		}

		Square sq = make_square(file, rank);
		effect_data[sq]++;

		if (pos.GetPiece(sq) != Piece::NO_PIECE)
		{
			// 駒があればそこで終了
			break;
		}
	}
}


/*-----------------------------------------------------------------------------*/
/**
* @brief 打ち歩詰めチェック
*/
/*-----------------------------------------------------------------------------*/
template<Color Us>
bool MoveCheck::IsPawnDropMate(const Position& pos, const Move& move)
{
	Position pos_next = pos;

	if (!pos_next.DoMove(move))
	{
		// 盤面動かすのに失敗？

		return false;
	}

	// 指した手の効きに相手の玉がある => 王手した
	if (!MoveCheck::IsCheck<Us>(pos_next, move.to()))
	{
		return false;
	}

	int effect_data[SQUARE_NB];

	MoveCheck::ClearEffectData(effect_data);

	// 効きデータの作成
	MakeEffectAll<Us>(pos_next, effect_data, Piece::NO_PIECE);

	// 指した先に先手の効きがあるか？
	if (effect_data[move.to()] == 0)
	{
		// 効きがないよ
		return false;
	}

	// 後手玉の移動先に先手の効きがあるか？

	// 相手玉の位置を取得
	const Color opp_color = opp(Us);
	const Piece king = make_piece(opp_color, KING);

	Square sq;
	if ((sq = pos_next.SearchPiece(king)) == SQUARE_NB)
	{
		// 相手玉がないよ
		return false;
	}

	Rank king_rank = rank_of(sq);
	File king_file = file_of(sq);

	// 相手玉の移動先の先手効きを調べる
	for (auto& ofs : GyokuMoveOfs)
	{
		Rank rank;
		File file;

		if (Us == BLACK)
		{
			rank = Rank(king_rank - ofs.rank);
			file = File(king_file - ofs.file);
		}
		else
		{
			rank = Rank(king_rank + ofs.rank);
			file = File(king_file + ofs.file);
		}

		if (Position::InBoard(file, rank) && !(pos_next.ColorOf(file, rank) == opp_color))
		{
			// ボード内で自分の駒以外の場所の全部の効きチェック

			if (effect_data[make_square(file, rank)] == 0)
			{
				// 移動先に効きはないそうよ
				return false;
			}
		}
	}

	bool ret;
	
	if (Us == BLACK)
	{
		ret = MoveCheck::response<WHITE>(pos_next, move.to(), king);
	}
	else
	{
		ret = MoveCheck::response<BLACK>(pos_next, move.to(), king);
	}

	if (ret)
	{
		// 応手がある
		return false;
	}

	// ここまでくれば打ち歩詰み
	return true;
}

/*-----------------------------------------------------------------------------*/
/**
* @brief 王手に対する応手
*/
/*-----------------------------------------------------------------------------*/
template<Color Us>
bool MoveCheck::response(Position& pos, Square to_sq, Piece skip_piece)
{
	Piece piece;
	Square sq;
	int effect_data[SQUARE_NB];

	Piece cap_piece = pos.GetPiece(to_sq);

	for (sq = SQ_ZERO; sq < SQUARE_NB; sq += 1)
	{
		piece = pos.GetPiece(sq);

		if (color_of(piece) == Us && skip_piece != piece)
		{
			// 先手の駒がある
			// 利きデータの作成
			MoveCheck::ClearEffectData(effect_data);
			MakeEfectOne<Us>(pos, sq, effect_data);

			if (effect_data[to_sq] != 0)
			{
				Move movedata(MoveType::NORMAL, sq, to_sq, piece, cap_piece);
				pos.DoMove(movedata);

				if (!MoveCheck::IsMateLeft<Us>(pos))
				{
					return true;
				}

				pos.UndoMove(movedata);
			}
		}
	}

	return false;
}


/*-----------------------------------------------------------------------------*/
/**
* @brief 王手判定
*/
/*-----------------------------------------------------------------------------*/
template<Color Us>
bool MoveCheck::IsCheck(const Position& pos, Square square)
{
	Piece piece;

	piece = pos.GetPiece(square);

	int effect_data[SQUARE_NB];

	MoveCheck::ClearEffectData(effect_data);

	// 指した駒の効きデータを作成
	MoveCheck::MakeEfectOne<Us>(pos, square, effect_data);

	// 相手玉の位置を取得
	Piece king = make_piece(opp(Us), KING);

	int sq = pos.SearchPiece(king);

	if (sq < SQUARE_NB)
	{
		// 後手の玉に先手の利きがある
		if (effect_data[sq] != 0)
		{
			return true;
		}
	}

	return false;
}


/*-----------------------------------------------------------------------------*/
/**
 * @brief 相対位置で移動動作機があるかどうか
 * @param 
 * @note  
 */
/*-----------------------------------------------------------------------------*/
bool MoveCheck::CanMove(const MoveCoord move_ofs[], int num, int file_ofs, int rank_ofs)
{
	for (int i = 0; i < num; i++)
	{
		const MoveCoord &ofs = move_ofs[i];

		if (ofs.rank == rank_ofs && ofs.file == file_ofs)
		{
			// 一致
			return true;
		}
	}

	return false;
}

/*-----------------------------------------------------------------------------*/
/**
 * @brief 縦方向の移動チェック
 * @note  
 */
/*-----------------------------------------------------------------------------*/
bool MoveCheck::CanMoveRank(const Position& pos, const Move& move)
{
	int ofs;

	// Debug.Assert(move.to() != move.from(), "引数エラー");

	// fileのチェック
	if (file_of(move.to()) != file_of(move.from()))
	{
		return false;
	}

	if (move.to() > move.from())
	{
		ofs = FILE_NB;
	}
	else
	{
		ofs = -FILE_NB;
	}

	Square sq = Square(move.from() + ofs);

	for (; sq != move.to(); sq += ofs)
	{
		if (pos.GetPiece(sq) != Piece::NO_PIECE)
		{
			// 指定場所に行くまでに駒があった場合

			return false;
		}
	}

	return true;
}


/*-----------------------------------------------------------------------------*/
/**
* @brief 横方向の移動チェック
* @note
*/
/*-----------------------------------------------------------------------------*/
bool MoveCheck::CanMoveFile(const Position& pos, const Move& move)
{
	int ofs;

	// Debug.Assert(move.to() != move.from(), "引数エラー");

	// rankが違う場合はエラー
	if (rank_of(move.to()) != rank_of(move.from()))
	{
		return false;
	}

	if (move.to() > move.from())
	{
		ofs = 1;
	}
	else
	{
		ofs = -1;
	}

	Square sq = Square(move.from() + ofs);

	for (; sq != move.to(); sq += ofs)
	{
		if (pos.GetPiece(sq) != Piece::NO_PIECE)
		{
			// 指定場所に行くまでに駒があった場合

			return false;
		}
	}

	return true;
}

/*-----------------------------------------------------------------------------*/
/**
 * @brief 斜めの移動チェック
 */
/*-----------------------------------------------------------------------------*/
bool MoveCheck::CanMoveDiagonal(const Position& pos, const Move& move)
{
	// Debug.Assert(move.to() != move.from(), "引数エラー");

	int rank_ofs, file_ofs;
	int ofs;

	file_ofs = file_of(move.to()) - file_of(move.from());
	rank_ofs = rank_of(move.to()) - rank_of(move.from());

	if (abs(file_ofs) != abs(rank_ofs))
	{
		return false;
	}

	rank_ofs = rank_ofs / abs(rank_ofs);
	file_ofs = file_ofs / abs(file_ofs);

	ofs = (rank_ofs * FILE_NB) + file_ofs;

	Square sq = Square(move.from() + ofs);

	while (move.to() != sq)
	{
		if (!in_board(sq))
		{
			// 一応
			return false;
		}

		if (pos.GetPiece(sq) != Piece::NO_PIECE)
		{
			// 指定場所に行くまでに駒があった場合

			return false;
		}

		sq += ofs;
	}

	return true;
}

/*-----------------------------------------------------------------------------*/
/**
 * @brief 先手側の王手放置判定
 * @param 
 * @note  
 */
/*-----------------------------------------------------------------------------*/
template<Color Us>
bool MoveCheck::IsMateLeft(const Position& pos)
{
	int effect_data[SQUARE_NB];

	MoveCheck::ClearEffectData(effect_data);

	const Piece king = make_piece(Us, KING);
	Square sq;

	if ((sq = pos.SearchPiece(king)) == SQUARE_NB)
	{
		// 玉ないあるよ
		return false;
	}

	// 後手の効きでーた作成
	if (Us == BLACK)
	{
		MakeEffectAll<WHITE>(pos, effect_data, Piece::NO_PIECE);
	}
	else
	{
		MakeEffectAll<BLACK>(pos, effect_data, Piece::NO_PIECE);
	}

	// 先手玉に後手の効きがあるか？
	if (effect_data[sq] == 0)
	{
		// 効きがないよ
		return false;
	}

	return true;
}


/*-----------------------------------------------------------------------------*/
/**
 * @brief 成り判定
 * @note  
 */
/*-----------------------------------------------------------------------------*/
bool MoveCheck::CanPromota(const Move& move)
{
	bool ret = false;

	if (!(move.move_type() & MOVE_FLAG))
	{
		// 盤上の駒を動かす手のみ有効
		return false;
	}

	if (move.move_type() & DROP_FLAG)
	{
		return false;
	}

	Piece piece = move.piece();

	if (is_promoted(piece))
	{
		// 成り駒は成れません
		return false;
	}

	if (type_of(piece) == PieceType::GOLD || type_of(piece) == PieceType::KING)
	{
		// 王と金は成れません
		return false;
	}

	if (color_of(piece) == BLACK)
	{
		if (rank_of(move.to()) <= RANK_3 || rank_of(move.from()) <= RANK_3)
		{
			ret = true;
		}
	}
	else
	{
		if (rank_of(move.to()) >= RANK_7 || rank_of(move.from()) >= RANK_7)
		{
			ret = true;
		}
	}

	return ret;
}


/*-----------------------------------------------------------------------------*/
/**
* @brief 強制成り判定
* @note
*/
/*-----------------------------------------------------------------------------*/
bool MoveCheck::ForcePromotion(Piece piece, Square square)
{
	if (piece == Piece::B_PAWN || piece == Piece::B_LANCE)
	{
		// 先手
		if (rank_of(square) <= Rank::RANK_1)
		{
			return true;
		}
	}
	else if (piece == Piece::W_PAWN || piece == Piece::W_LANCE)
	{
		// 後手
		if (rank_of(square) >= Rank::RANK_9)
		{
			// 9段目には打てません
			return true;
		}
	}
	else if (piece == Piece::B_KNIGHT)
	{
		// 先手
		if (rank_of(square) <= Rank::RANK_2)
		{
			// 1,2段目には打てません
			return true;
		}
	}
	else if (piece == Piece::W_KNIGHT)
	{
		// 後手
		if (rank_of(square) >= Rank::RANK_8)
		{
			// 8,9段目には打てません
			return true;
		}
	}

	return false;
}
