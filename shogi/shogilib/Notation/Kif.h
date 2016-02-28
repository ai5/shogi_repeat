#pragma once
#ifndef SHOGI_KIF_H_
#define SHOGI_KIF_H_

#include <string>
#include <map>

#include "types.h"
#include "Handicap.h"
#include "Move.h"

class Notation;
class Position;

class Kif
{
public:
	Kif();
	~Kif();

	virtual bool Load(Notation& notation, const std::string& filename);
	virtual bool Save(Notation& notation, const std::string& filename);
	virtual bool CanRead(const std::string& str);

	virtual void LoadFromString(Notation& notation, const std::string& str);
	virtual std::string  SaveToString(const Notation& notation);

	static std::string ToBodString(const Notation& notation);

	static std::wstring ToMoveString(const Move& move);
	static std::wstring ToMoveTimeString(const MoveKif& info);

protected:
	// 読み込み時に使う
	Rank pos_rank_ = RANK_1;

	// 書き込み時に使う
	const wchar_t** turn_str_;

	void ReadNotation(Notation& notation, std::istream& sr);
	bool ReadHeader(Notation& notation, const std::wstring& line);
	bool ReadPosition(Notation&notation, Position& position, const std::wstring& line);
	bool ReadMove(Notation& notation, const std::wstring& line);
	bool ReadHand(Color color, Position& position, const std::wstring& line, int ofs);
	bool ReadKifMove(Notation& notation, const std::wstring& line);

	void WriteNotation(const Notation& notation, std::ostream& wr);
	static void WritePosition(const Position& pos, Handicap handicap, std::ostream& wr);
	void WriteMoveTree(const Moves& moves, std::ostream& wr);
	void WriteMoves(const Moves& moves, std::ostream& wr);

	static const wchar_t* PieceCharFromPiece(Piece piece);
	static const wchar_t* PieceStrFromPiece(Piece piece);

	static std::wstring HandStrFromHand(const int hand[PIECE_TYPE_NB]);

	static const wchar_t* MoveTypeStrFromMoveType(MoveType movetype);

	static Piece PieceFromWStr(const std::wstring& str, size_t* pos);
	static int NumberFromKanjiNumber(std::wstring str, size_t* index);
	static int NumberFromWChar(wchar_t ch);

	static bool IsMoveSeparator(wchar_t ch);
	static bool IsMoveSeparatorEnd(wchar_t ch);
	static bool IsSankaku(wchar_t ch);
	static void SplitMove(std::deque<std::wstring>& str_list, const std::wstring& line);

	static bool ParseKifMove(const Notation& notation, const std::wstring&, MoveKif& move);
	
	// sjis用 -> unicodeに変更
	static const wchar_t* ArabiaNumber[];
	static const wchar_t* KanNumber[];
	static const wchar_t* KanjiPiece[];
	static const wchar_t* KanjiPiecePromotion[];
	static const wchar_t* KanjiPiecePromotionStr[];
	static const wchar_t* TurnStrSengo[];
	static const wchar_t* TurnStrSimokami[];
	static const std::map<MoveType, const wchar_t*> ResultTable;

	// unicode用
	static const std::map<std::wstring, Piece> WidePieceTable;
	static const std::wstring WideKanjiNumber;
	static const std::wstring WideArabicNumber;
	static const std::map<std::wstring, MoveType> ResultFromWStringTable;
};

#endif
