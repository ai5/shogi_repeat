#pragma once

#ifndef SHOGI_SFEN_H_
#define SHOGI_SFEN_H_

#include <string>
#include "types.h"
#include "Move.h"

class Notation;
class Position;

class Sfen
{
private:
	static const PieceType handPieceType[];

public:
	Sfen();
	~Sfen();

	void Load(Notation& notation, const std::string& filename);
	void Save(Notation& notation, const std::string& filename);
	bool CanRead(const std::string& str);

	void LoadFromString(Notation& notation, const std::string& sfen);
	std::string  SaveToString(const Notation& notation);

	static void LoadNotation(Notation& notation, const std::string& sfen);
	static void LoadPosition(Position& position, const std::string& sfen);

	static std::string PositionToString(const Position& position, int num);
	static std::string MovesToString(const Notation& notation);
	static std::string MoveToString(const Move& move);

	static bool IsSfen(std::string str);


	static Move ParseMove(const Position& position, const std::string& move);
	static PieceType ConvPieceTypeFromChar(char ch);

private:
	static void ReadNotation(Notation& notation, const std::string& sfen);
	static void ReadPosition(Position& position, const std::string& sfen);

	static void WriteNotation(const Notation& notation, std::ostream& wr);
	static void WritePosition(const Position& pos, std::ostream& wr, int number);
	static void WriteMoves(const Notation& notation, std::ostream& wr);
	static void WriteMove(const Move& move, std::ostream& wr);
	static void WriteMoves(const Notation& notation, std::ostream& wr, int number);

	static char ConvCharFromPieceType(PieceType pieceType);

	static File FileFromChar(char ch);
	static Rank RankFromChar(char ch);

};

#endif