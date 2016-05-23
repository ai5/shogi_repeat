#pragma once

#ifndef SHOGI_MOVE_CHECK_H_
#define SHOGI_MOVE_CHECK_H_

#include "types.h"
#include "Move.h"

class Position;

struct MoveCoord 
{
	int file; // オフセット
	int rank; // オフセット
};

class MoveCheck
{
	static const MoveCoord FuMoveOfs[];
	static const MoveCoord KeiMoveOfs[];
	static const MoveCoord GinMoveOfs[];
	static const MoveCoord KinMoveOfs[];
	static const MoveCoord GyokuMoveOfs[];


public:

	static bool IsValid(const Position& pos, const Move& move);
	static bool IsCheck(const Position& pos);
	static bool CanPromota(const Move& move);
	static bool ForcePromotion(Piece piece, Square square);

protected:
	static bool MoveCheckParam(const Position& pos, const Move& move);
	static bool MoveCheckDrop(const Position& pos, const Move& move);
	static bool MoveCheckMove(const Position& pos, const Move& move);

	static bool IsMateLeft(const Position& pos, const Move& move);

	static int PieceCountInFile(const Position& pos, Piece piece, File file);

	static void ClearEffectData(int effect_dataa[SQUARE_NB]);
	

	template<Color Us>
	static void MakeEffectAll(const Position& pos, int effect_data[SQUARE_NB], Piece skip_piece);

	template<Color Us>
	static void MakeEfectOne(const Position& pos, Square square, int effect_data[SQUARE_NB]);

	static void MakeEffect(Color side, int effect_data[SQUARE_NB], const MoveCoord move_ofs[], size_t num, File move_to_file, Rank move_to_rank);
	static void MakeEffect(const Position& pos, int effect_data[SQUARE_NB], File file, Rank rank, int file_ofs, int rank_ofs);

	template<Color Us>
	static bool IsCheck(const Position& pos, Square square);

	template<Color Us>
	static bool IsPawnDropMate(const Position& pos, const Move& move);

	template<Color Us>
	static bool response(Position& pos, Square to_sq, Piece skip_piece);


	static bool CanMove(const MoveCoord move_ofs[], int num, int file_ofs, int rank_ofs);
	static bool CanMoveRank(const Position& pos, const Move& move);
	static bool CanMoveFile(const Position& pos, const Move& move);
	static bool CanMoveDiagonal(const Position& pos, const Move& move);

	template<Color Us>
	static bool IsMateLeft(const Position& pos);

};

#endif
