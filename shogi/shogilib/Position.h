#pragma once

#ifndef SHOGI_POSITION_H_
#define SHOGI_POSITION_H_

#include "types.h"
#include "Key.h"
#include "Handicap.h"

class Position
{
private:
	Board board_;
	Hand hand_;

	Color side_to_move_;
	Key hashkey_;

public:
	Position();
	~Position();

	void Init();
	void InitHashKey();

	Color side_to_move() const { return this->side_to_move_; }
	void set_side_to_move(Color side) { 
		this->side_to_move_ = side;
		this->InitHashKey(); 
	}

	const Board& board() const {
		return this->board_;
	}

	Piece GetPiece(Square sq) const { return this->board_[sq]; }
	Piece GetPiece(File file, Rank rank) const { return this->board_[make_square(file, rank)]; }

	void SetPiece(Square sq, Piece piece) {	this->board_[sq] = piece; }
	void SetPiece(File file, Rank rank, Piece piece) { this->board_[make_square(file, rank)] = piece; }

	const Hand& hand() const {	return this->hand_;	}

	bool IsBlack(File file, Rank rank) const { return color_of(this->board_[make_square(file, rank)]) == BLACK; }
	bool IsWhite(File file, Rank rank) const { return color_of(this->board_[make_square(file, rank)]) == WHITE; }

	Color ColorOf(File file, Rank rank) const {
		return color_of(this->board_[make_square(file, rank)]);
	}
	

	bool IsEmpty(File file, Rank rank) const { return this->board_[make_square(file, rank)] == Piece::NO_PIECE; }
	bool IsHand(Color color, PieceType piece_type) const { return this->hand_[color][piece_type] != 0; }

	void SetBlackHand(PieceType pt, int num)
	{
		this->hand_[BLACK][pt] = num;
	}

	void SetWhiteHand(PieceType pt, int num)
	{
		this->hand_[WHITE][pt] = num;
	}

	const Key& hashkey() const {
		return this->hashkey_;
	}

	void SetHandicapKyo();
	void SetHandicapRightKyo();
	void SetHandicapKaku();
	void SetHandicapHisya();
	void SetHandicapHiKyo();
	void SetHandicap2();
	void SetHandicap3();
	void SetHandicap4();
	void SetHandicap5();
	void SetHandicapLeft5();
	void SetHandicap6();
	void SetHandicap8();
	void SetHandicap10();


	void Reverse();
	void BoardClear();

	bool DoMove(const Move& move);
	bool UndoMove(const Move& move);

	bool Equals(const Position& pos) const;

	Square SearchPiece(Piece piece) const;

	static bool InBoard(int file, int rank)
	{
		if ((file >= 0 && file < File::FILE_NB)
			&& (rank >= 0 && rank < Rank::RANK_NB))
		{
			return true;
		}

		return false;
	}

private:
	void resetBoard();
};

#endif
