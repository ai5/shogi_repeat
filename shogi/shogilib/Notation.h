#pragma once

#ifndef SHOGI_NOTATION_H_
#define SHOGI_NOTATION_H_

#include <string>
#include <map>
#include "types.h"

#include "position.h"
#include "move.h"

class Notation
{
private:
	Position position_;
	Position initial_position_;

	std::string black_name_;
	std::string white_name_;

	Handicap handicap_ = Handicap::HIRATE;
	bool output_initial_position_ = false;

	std::map<std::string, std::string> kifu_infos_;

	Moves moves_;

	Moves* current_moves_;
	MoveKif* move_current_;

	MoveType result_ = NO_MOVE;
	Color winner_ = NO_COLOR;

public:
	Notation();
	Notation(const Notation& notation) {
		this->position_         = notation.position_;
		this->initial_position_ = notation.initial_position_;
		this->black_name_       = notation.black_name_;
		this->white_name_       = notation.white_name_;
		this->handicap_         = notation.handicap_;
		this->output_initial_position_ = this->output_initial_position_;
		this->kifu_infos_       = notation.kifu_infos_;

		this->result_ = notation.result_;
		this->winner_ = notation.winner_;

		this->moves_ = notation.moves_;
		MoveKif::ChangeParent(this->moves_, nullptr);

		this->current_moves_ = &this->moves_;
		this->move_current_ = &this->moves_[0];

		this->Jump(notation.number());
	}

	~Notation();

	void Init();
	void InitHashKey();

	// 初期位置設定
	void SetInitialPosition(const Position& pos);
	void DecisionHandicap();
	
	const Position& position() const{
		return this->position_;
	}

	const Position& initial_position() const {
		return this->initial_position_;
	}

	const std::string& black_name() const { return this->black_name_; }
	void set_black_name(const std::string& black_name) { this->black_name_ = black_name; }

	const std::string& white_name() const { return this->white_name_; }
	void set_white_name(const std::string& white_name) { this->white_name_ = white_name; }

	Handicap handicap() const {	return this->handicap_;	}
	void set_handicap(Handicap handicap);

	bool output_initial_position() const { return this->output_initial_position_; }

	const std::map<std::string, std::string>& kifu_infos() const { return this->kifu_infos_; }
	void set_kifu_info(const std::string& key, const std::string& value) {
		this->kifu_infos_[key] = value;
	}

	int number() const { return this->move_current_->number(); }
	const MoveKif& move_current() const { return *this->move_current_; }

	MoveType result() const { return this->result_; }
	Color winner() const { return this->winner_; }

	const Moves& moves() const { return this->moves_; }

	// 移動関連
	void Jump(int num);
	void First();
	void Last();
	bool Prev(int num);
	bool Next(int num);


	// 指し手追加
	bool AddMove(const MoveKif& move);
	bool MergeMove(const MoveKif& move);

	// 対局最下位
	void Continue(bool del_stop);

	// ブランチ
	void AddBranch(const Moves& moves);
	void DeleteBranch(int no);
	void SwapBranch(int left, int right);

	void ChangeChildCurrent(int no);

private:
	const MoveKif*  Notation::getPrevMove() const;
	const MoveKif* Notation::getNext() const;

	Color getWinPlayer(const Move& move) const;

	void CopyMoves(const Notation& notation);
};

#endif

