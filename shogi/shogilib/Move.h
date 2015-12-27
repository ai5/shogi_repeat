#pragma once

#ifndef SHOGI_MOVE_H_
#define SHOGI_MOVE_H_

#include <list>
#include <string>
#include <vector>

#include "types.h"

enum MoveType {
	NO_MOVE,
	PASS,

	MOVE_FLAG = 0x10,
	DROP_FLAG = 0x20,
	RESULT_FLAG = 0x40,


	NORMAL = MOVE_FLAG,   // 通常指手
	PROMOTION_FLAG = 1,   // 成りフラグ
	UNPROMOTION_FLAG = 2, // 不成ふらぐ
	CAPTURE_FLAG = 4,     // コマを取った
	SAME_FLAG = 8,        // 同ほげ

	CAPTURE = MOVE_FLAG | CAPTURE_FLAG,

	MOVE_MASK = (MOVE_FLAG | PROMOTION_FLAG),

	DROP = MOVE_FLAG | DROP_FLAG,

	RESIGN = RESULT_FLAG,  // 投了
	STOP,       // 中断
	REPETITION, // 千日手
	DRAW,       // 持将棋
	TIMEOUT,    // 切れ負け
	MATE,       // 詰み
	NO_MATE,     // 不詰み
	LOSE_FOUL,   // 反則負け
	WIN_FOUL,    // 反則勝ち
	NYUGYOKU_KACHI, // 入玉勝ち
};


class MoveKif;
using Moves = std::vector<MoveKif>;


class Move
{
protected:
	Color side_; // 手番
	Square to_;
	Square from_;
	MoveType move_type_;
	Piece piece_;
	Piece capture_piece_;

public:

public:
	Move() {
		this->Init();
	}

	Move(MoveType move_type) {
		this->Init();

		this->move_type_ = move_type;
	}

	Move(const Move& move)
	{
		this->to_ = move.to_;
		this->from_ = move.from_;
		this->move_type_ = move.move_type_;
		this->piece_ = move.piece_;
		this->capture_piece_ = move.capture_piece_;
		this->side_ = move.side_;
	}


	virtual ~Move();

	void Init();
	bool IsNoMove() const { return this->move_type_ == MoveType::NO_MOVE; }

	Square to() const { return this->to_; }
	void set_to(Square to) { this->to_ = to; }

	Square from() const { return this->from_; }
	void set_from(Square from) { this->from_ = from; }

	MoveType move_type() const { return this->move_type_; }
	void set_move_type(MoveType move_type) { this->move_type_ = move_type; }

	Piece piece() const { return this->piece_; }
	void set_piece(Piece piece) { 
		this->piece_ = piece; 
	}

	Piece capture_piece() const { return this->capture_piece_; }
	void set_capture_piece(Piece capture_piece) { this->capture_piece_ = capture_piece; }

	Color side() const { return this->side_; }
	void set_side(Color turn) {	this->side_ = turn;	}

	bool Equals(const Move& move) const;
};


// 棋譜の指し手
class MoveKif : public Move
{
protected:
	std::list<std::string> comment_list_; // コメント
	std::vector<Moves> branches_; // 分岐
	int current_; // 分岐のカレント

	// 分岐先頭要素の場合は親
	Moves* parent_;

	int number_;

public:
	int time;
	int totalTime;

	MoveKif() {
		this->Init();
	}

	MoveKif(const Move& move) : Move(move)
	{
		this->current_ = -1;
		this->parent_ = nullptr;
		this->number_ = 0;
		this->time = 0;
		this->totalTime = 0;
	}

	MoveKif(const MoveKif& move) : Move(move)
	{
		this->comment_list_ = move.comment_list_;
		this->branches_ = move.branches_;
		
		this->current_ = move.current_;
		this->parent_ = move.parent_;

		this->time = move.time;
		this->totalTime = move.totalTime;
		this->number_ = move.number_;
	}

	MoveKif(MoveType move_type) : Move(move_type)
	{
		this->current_ = -1;
		this->parent_ = nullptr;
		this->number_ = 0;
		this->time = 0;
		this->totalTime = 0;
	}


	~MoveKif();

	void Init();

	
	int getCurrentBranchNumber() const { return this->current_;	}
	void setCurrentBranchNumber(int value) { 
		// ASSERT_MSG(value >= (int)this->branches_.size(), "サイズオーバー");
		this->current_ = value;
	}

	const Moves* getCurrentBranch() const {
		if (this->current_ == -1)
		{
			return nullptr;
		}

		return &this->branches_[this->current_];
	}

	Moves* getCurrentBranch() {
		if (this->current_ == -1)
		{
			return nullptr;
		}

		return &this->branches_[this->current_];
	}

	const std::vector<Moves>& branches() const { return this->branches_; }

	int getParentNumber() const {
		return this->number_ - 1;
	}

	Moves* parent() {
		return this->parent_;
	}

	const Moves* parent() const {
		return this->parent_;
	}

	int number() const { return this->number_; }
	void set_number(int number) { this->number_ = number; }


	Moves* MakeBranch(Moves* parent);
	void AddBranch(Moves* parent, const MoveKif& move);
	void AddBranch(Moves* parent, const Moves& moves);
	void SwapBranch(int left, int right);
	void DeleteBranch(int no);
	void SwapParent(int no, const Moves& moves);

	static void ChangeParent(Moves& moves, Moves* parent);
};


inline bool is_move(MoveType mt)
{
	return (mt != NO_MOVE && !(mt & RESULT_FLAG));
}

inline bool is_result(MoveType mt)
{
	return (mt & RESULT_FLAG) != 0;
}


#endif

