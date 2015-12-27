#include <gtest/gtest.h>

#include <types.h>


TEST(PieceTest, testTypeOf)
{
	ASSERT_EQ(type_of(Piece::B_PAWN), PieceType::PAWN);
	ASSERT_EQ(type_of(Piece::W_PAWN), PieceType::PAWN);
	ASSERT_EQ(type_of(Piece::B_LANCE), PieceType::LANCE);
	ASSERT_EQ(type_of(Piece::W_LANCE), PieceType::LANCE);
	ASSERT_EQ(type_of(Piece::B_KING), PieceType::KING);
	ASSERT_EQ(type_of(Piece::W_KING), PieceType::KING);
	ASSERT_EQ(type_of(Piece::B_GOLD), PieceType::GOLD);
	ASSERT_EQ(type_of(Piece::W_GOLD), PieceType::GOLD);

	ASSERT_EQ(type_of(Piece::B_PRO_PAWN), PieceType::PAWN);
	ASSERT_EQ(type_of(Piece::W_PRO_LANCE), PieceType::LANCE);
	ASSERT_EQ(type_of(Piece::B_PRO_KNIGHT), PieceType::KNIGHT);
	ASSERT_EQ(type_of(Piece::W_PRO_SILVER), PieceType::SILVER);
	ASSERT_EQ(type_of(Piece::B_PRO_BISHOP), PieceType::BISHOP);
	ASSERT_EQ(type_of(Piece::W_PRO_ROOK), PieceType::ROOK);
}


TEST(PieceTest, testColorOf)
{
	ASSERT_EQ(color_of(Piece::B_PRO_PAWN), Color::BLACK);
	ASSERT_EQ(color_of(Piece::W_PRO_LANCE), Color::WHITE);
	ASSERT_EQ(color_of(Piece::B_PRO_KNIGHT), Color::BLACK);
	ASSERT_EQ(color_of(Piece::W_PRO_SILVER), Color::WHITE);
	ASSERT_EQ(color_of(Piece::B_PRO_BISHOP), Color::BLACK);
	ASSERT_EQ(color_of(Piece::W_PRO_ROOK), Color::WHITE);

}

TEST(PieceTest, testMakePiece)
{

	ASSERT_EQ(make_piece(Color::BLACK, PieceType::PAWN), Piece::B_PAWN);
	ASSERT_EQ(make_piece(Color::WHITE, PieceType::PAWN), Piece::W_PAWN);

	ASSERT_EQ(make_piece(Color::WHITE, PieceType::BISHOP), Piece::W_BISHOP);
	ASSERT_EQ(make_piece(Color::BLACK, PieceType::ROOK), Piece::B_ROOK);


	ASSERT_EQ(make_piece(Color::WHITE, PieceType::BISHOP, true), Piece::W_PRO_BISHOP);
	ASSERT_EQ(make_piece(Color::BLACK, PieceType::ROOK, true), Piece::B_PRO_ROOK);
}

TEST(PieceTest, testIsPromoted)
{
	ASSERT_EQ(is_promoted(Piece::B_PAWN), false);
	ASSERT_EQ(is_promoted(Piece::W_PAWN), false);
	ASSERT_EQ(is_promoted(Piece::B_LANCE), false);
	ASSERT_EQ(is_promoted(Piece::W_LANCE), false);
	ASSERT_EQ(is_promoted(Piece::B_KING), false);
	ASSERT_EQ(is_promoted(Piece::W_KING), false);
	ASSERT_EQ(is_promoted(Piece::B_GOLD), false);
	ASSERT_EQ(is_promoted(Piece::W_GOLD), false);

	ASSERT_EQ(is_promoted(Piece::B_PRO_PAWN), true);
	ASSERT_EQ(is_promoted(Piece::W_PRO_LANCE), true);
	ASSERT_EQ(is_promoted(Piece::B_PRO_KNIGHT), true);
	ASSERT_EQ(is_promoted(Piece::W_PRO_SILVER), true);
	ASSERT_EQ(is_promoted(Piece::B_PRO_BISHOP), true);
	ASSERT_EQ(is_promoted(Piece::W_PRO_ROOK), true);

}

TEST(PieceTest, testIsWhite)
{
	ASSERT_EQ(is_white(Piece::B_PAWN), false);
	ASSERT_EQ(is_white(Piece::W_PAWN), true);
	ASSERT_EQ(is_white(Piece::B_LANCE), false);
	ASSERT_EQ(is_white(Piece::W_LANCE), true);
	ASSERT_EQ(is_white(Piece::B_KING), false);
	ASSERT_EQ(is_white(Piece::W_KING), true);
	ASSERT_EQ(is_white(Piece::B_GOLD), false);
	ASSERT_EQ(is_white(Piece::W_GOLD), true);

	ASSERT_EQ(is_white(Piece::B_PRO_PAWN), false);
	ASSERT_EQ(is_white(Piece::W_PRO_LANCE), true);
	ASSERT_EQ(is_white(Piece::B_PRO_KNIGHT), false);
	ASSERT_EQ(is_white(Piece::W_PRO_SILVER), true);
	ASSERT_EQ(is_white(Piece::B_PRO_BISHOP), false);
	ASSERT_EQ(is_white(Piece::W_PRO_ROOK), true);

}

TEST(PieceTest, testOpp)
{

	ASSERT_EQ(opp(Piece::B_PAWN), Piece::W_PAWN);
	ASSERT_EQ(opp(Piece::B_KING), Piece::W_KING);
	ASSERT_EQ(opp(Piece::B_PRO_ROOK), Piece::W_PRO_ROOK);
	ASSERT_EQ(opp(Piece::B_BISHOP), Piece::W_BISHOP);
}

