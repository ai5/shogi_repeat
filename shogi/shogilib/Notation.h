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

	std::wstring black_name_;
	std::wstring white_name_;

	Handicap handicap_ = Handicap::HIRATE;
	bool output_initial_position_ = false;

	std::map<std::wstring, std::wstring> kifu_infos_;

	Moves moves_;

	Moves* current_moves_;
	MoveKif* move_current_;

	MoveType result_ = NO_MOVE;
	Color winner_ = NO_COLOR;

public:
	Notation();
	Notation(const Notation& notation);

	~Notation();
	Notation& operator=(const Notation& moves);

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

	const std::wstring& black_name() const { return this->black_name_; }
	void set_black_name(const std::wstring& black_name) { this->black_name_ = black_name; }

	const std::wstring& white_name() const { return this->white_name_; }
	void set_white_name(const std::wstring& white_name) { this->white_name_ = white_name; }

	Handicap handicap() const {	return this->handicap_;	}
	void set_handicap(Handicap handicap);

	bool output_initial_position() const { return this->output_initial_position_; }

	const std::map<std::wstring, std::wstring>& kifu_infos() const { return this->kifu_infos_; }
	void set_kifu_info(const std::wstring& key, const std::wstring& value) {
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

	void Back();

	// 対局最下位
	void Continue(bool del_stop);

	// ブランチ
	void AddBranch(const Moves& moves);
	void MergeNotation(const Notation& notation);
	void DeleteBranch(int no);
	void SwapBranch(int left, int right);
	void SwapBranch(MoveKif* move, Moves* moves, int left, int right);

	void ChangeChildCurrent(int no);
	void DeleteNotCurrent();

	// カレントにコメント追加
	void AddComment(std::wstring str) {
		this->move_current_->AddComment(str);
	}

	void SetComments(KifComments comments) {
		this->move_current_->set_comments(comments);
	}

private:
	const MoveKif*  Notation::getPrevMove() const;
	const MoveKif* Notation::getNext() const;

	Color getWinPlayer(const Move& move) const;

};

#endif

