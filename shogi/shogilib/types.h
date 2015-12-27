#pragma once

#ifndef SHOGI_TYPES_H_
#define SHOGI_TYPES_H_

#include <cassert>
#include <cctype>

#ifdef _DEBUG
#define ASSERT(X) { assert(X); }
#define ASSERT_MSG(X, Y) { assert(X); }
#else
#define ASSERT(X) {}
#define ASSERT_MSG(X, Y) {}
#endif

enum File {
	FILE_9, FILE_8, FILE_7, FILE_6, FILE_5, FILE_4, FILE_3, FILE_2, FILE_1, FILE_NB
};

enum Rank {
	RANK_1, RANK_2, RANK_3, RANK_4, RANK_5, RANK_6, RANK_7, RANK_8, RANK_9, RANK_NB
};

enum Color {
	BLACK, WHITE, NO_COLOR,
	COLOR_NB = 2,
};

enum Square {
	SQ_91, SQ_81, SQ_71, SQ_61, SQ_51, SQ_41, SQ_31, SQ_21, SQ_11,
	SQ_92, SQ_82, SQ_72, SQ_62, SQ_52, SQ_42, SQ_32, SQ_22, SQ_12,
	SQ_93, SQ_83, SQ_73, SQ_63, SQ_53, SQ_43, SQ_33, SQ_23, SQ_13,
	SQ_94, SQ_84, SQ_74, SQ_64, SQ_54, SQ_44, SQ_34, SQ_24, SQ_14,
	SQ_95, SQ_85, SQ_75, SQ_65, SQ_55, SQ_45, SQ_35, SQ_25, SQ_15,
	SQ_96, SQ_86, SQ_76, SQ_66, SQ_56, SQ_46, SQ_36, SQ_26, SQ_16,
	SQ_97, SQ_87, SQ_77, SQ_67, SQ_57, SQ_47, SQ_37, SQ_27, SQ_17,
	SQ_98, SQ_88, SQ_78, SQ_68, SQ_58, SQ_48, SQ_38, SQ_28, SQ_18,
	SQ_99, SQ_89, SQ_79, SQ_69, SQ_59, SQ_49, SQ_39, SQ_29, SQ_19,
	SQUARE_NB,

	SQ_NONE,
};


enum PieceType {
	NO_PIECE_TYPE, 
	PAWN, LANCE, KNIGHT, SILVER, BISHOP, ROOK, GOLD, KING,
	PIECE_TYPE_NB,
};

enum Piece {
	NO_PIECE,
	B_PAWN, B_LANCE, B_KNIGHT, B_SILVER, B_BISHOP, B_ROOK, B_GOLD, B_KING,
	B_PRO_PAWN, B_PRO_LANCE, B_PRO_KNIGHT, B_PRO_SILVER, B_PRO_BISHOP, B_PRO_ROOK,

	W_PAWN = 17, W_LANCE, W_KNIGHT, W_SILVER, W_BISHOP, W_ROOK, W_GOLD, W_KING,
	W_PRO_PAWN, W_PRO_LANCE, W_PRO_KNIGHT, W_PRO_SILVER, W_PRO_BISHOP, W_PRO_ROOK,
	PIECE_NB,

	PIECE_PROMOTION_OFS = 8,
	PIECE_WHITE_OFS = 16,
	PIECE_PROMOTION_MASK = 0xf
};

// stock fishのコピペ
#define ENABLE_BASE_OPERATORS_ON(T)                             \
inline T operator+(T d1, T d2) { return T(int(d1) + int(d2)); } \
inline T operator-(T d1, T d2) { return T(int(d1) - int(d2)); } \
inline T operator*(int i, T d) { return T(i * int(d)); }        \
inline T operator*(T d, int i) { return T(int(d) * i); }        \
inline T operator-(T d) { return T(-int(d)); }                  \
inline T& operator+=(T& d1, T d2) { return d1 = d1 + d2; }      \
inline T& operator-=(T& d1, T d2) { return d1 = d1 - d2; }      \
inline T& operator*=(T& d, int i) { return d = T(int(d) * i); }

#define ENABLE_FULL_OPERATORS_ON(T)                             \
ENABLE_BASE_OPERATORS_ON(T)                                     \
inline T& operator++(T& d) { return d = T(int(d) + 1); }        \
inline T& operator--(T& d) { return d = T(int(d) - 1); }        \
inline T operator/(T d, int i) { return T(int(d) / i); }        \
inline int operator/(T d1, T d2) { return int(d1) / int(d2); }  \
inline T& operator/=(T& d, int i) { return d = T(int(d) / i); }

// ENABLE_FULL_OPERATORS_ON(Piece)
ENABLE_FULL_OPERATORS_ON(Square)
ENABLE_FULL_OPERATORS_ON(File)
ENABLE_FULL_OPERATORS_ON(Rank)

#undef ENABLE_FULL_OPERATORS_ON
#undef ENABLE_BASE_OPERATORS_ON

inline File& operator+=(File& d1, int d2) { return d1 = File(d1 + d2); }
inline Rank& operator+=(Rank& d1, int d2) { return d1 = Rank(d1 + d2); }

inline File file_of(Square s)
{
	return File(s % FILE_NB);
}

inline Rank rank_of(Square s)
{
	return Rank(s / FILE_NB);
}

inline File file_from_suji(int suji)
{
	return File(FILE_NB - suji);
}

inline Rank rank_from_dan(int dan)
{
	return Rank(dan - 1);
}

inline int suji_from_file(File file)
{
	return File::FILE_NB - file;
}

inline int dan_from_rank(Rank rank)
{
	return rank + 1;
}

// 
inline PieceType type_of(Piece pc) {

	PieceType pt = PieceType(pc & 7);
	
	return (pt == NO_PIECE_TYPE) ? PieceType(pc & PIECE_PROMOTION_OFS) : pt;
}

inline Piece make_piece(Color c, PieceType pt) {
	return Piece((c << 4) | pt);
}

inline Piece make_piece(Color c, PieceType pt, bool promotion) {
	return Piece((c << 4) | pt | (promotion ? PIECE_PROMOTION_OFS : 0));
}

inline Piece opp(Piece piece) {
	return Piece(piece ^ PIECE_WHITE_OFS);
}

inline Color color_of(Piece pc) {
	if (pc == Piece::NO_PIECE)
	{
		return Color::NO_COLOR;
	}

	return Color(pc >> 4);
}

inline bool is_promoted(Piece pc) {
	return (pc & PIECE_PROMOTION_MASK) > PIECE_PROMOTION_OFS;
}

inline bool is_white(Piece pc) {
	return (pc > PIECE_WHITE_OFS);
}


inline Color opp(Color color)
{
	return Color(color ^ WHITE);
}

inline wchar_t to_char(Color color)
{
	return (color == WHITE) ? '△' : '▲';
}

inline Square make_square(File file, Rank rank)
{
	return Square(file + rank * (int)FILE_NB);
}

using Board = Piece[SQUARE_NB];
using Hand = int[COLOR_NB][PIECE_TYPE_NB];


#endif
