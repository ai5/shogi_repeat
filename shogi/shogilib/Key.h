#pragma once

#include <cstdint>

#ifndef SHOGI_KEY_H_
#define SHOGI_KEY_H_

#include "types.h"

class Move;

class Key
{
	uint64_t boardkey_;
	uint32_t hand_b_;
	uint32_t hand_w_;
public:
	static const uint64_t InitialBoardKey = 4038204047121054572;
	static const uint32_t TurnBit = 1;


	Key();
	Key(const Key& key);
	Key(Color side, const Board& board, const Hand& hand);
	Key(uint64_t boardkey, uint32_t hand_b, uint32_t hand_w);

	void Init(Color side, const Board& board, const Hand& hand);
	void DoMove(const Move& move);
	void UndoMove(const Move& move);

	bool Equals(const Key& key) const {
		return (this->boardkey_ == key.boardkey_) && (this->hand_b_ == key.hand_b_) && (this->hand_w_ == key.hand_w_);
	}

	uint64_t boardkey() const { return this->boardkey_;	}

	uint32_t hand_b() const { return this->hand_b_;	}

	uint32_t hand_w() const { return this->hand_w_; }

private:
	uint64_t to_hash(const Move& move) const;
	uint64_t from_hash(const Move& move) const;
	uint64_t cap_hash(const Move& move) const;

	uint32_t drophand(const Move& move) const;
	uint32_t caphand(const Move& move) const;

private:
	static const uint64_t zobBoard[PIECE_NB][SQUARE_NB];
	static const uint32_t handTable[PIECE_TYPE_NB];


};

inline bool operator==(const Key& left, const Key& right) {
	return left.Equals(right);
}

inline bool operator<(const Key& left, const Key& right) {
	return left.boardkey() < right.boardkey();
}


#endif


