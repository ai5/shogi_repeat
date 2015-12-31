#include <gtest/gtest.h>

#include "MoveCheck.h"
#include "Sfen.h"
#include "Position.h"
#include "Notation.h"

class MoveCheckTest : public ::testing::Test {

protected:
	static const std::string sfen;
	static const std::string sfen2;
	static const std::string moves[];
	static const bool promotions[];

	static const std::string sfen3;
	static const std::string sfen4;

	virtual void SetUp() {
	}

	// virtual void TearDown() {}

};

const std::string MoveCheckTest::sfen = "lnBB4l/kgRG5/1pp1pgn1p/p5p2/3NPP3/2P3P2/PPKp4P/L8/6GNL b R4S3Pp 115";
const std::string MoveCheckTest::sfen2 = "ln1B4l/k2G5/ppp1pgn1p/6p2/3NPP3/2P3P2/PPKp4P/L8/6GNL w 2RBG3S3Psp 104";
const std::string MoveCheckTest::moves[] = { "S*6h", "7g8f", "6h7g", "8f7g", "9c9d", "G*8b", "9b8b", "B*7a", "8b9b", "R*7b", "G*8b", "7b8b+", "resign" };
const bool MoveCheckTest::promotions[] = { false, false, true, false, false, false, false, false, false, false, false, true, false };

const std::string MoveCheckTest::sfen3 = "3B4l/k2G5/4pgn1p/6p2/3NPP3/2Pbs1P2/PP1p4P/LLK6/5rGN1 w RG3S3Pnl4p 1";
const std::string MoveCheckTest::sfen4 = "1ngR5/6kll/p4P1pp/2p1SBp2/3ppn3/2P6/P1NGP4/5g2K/L4b3 b NL4Prg3s3p 1";


// [TestMethod, Description("IsValid‚ÌƒeƒXƒg")]
TEST_F(MoveCheckTest, testIsValid)
{
	Position pos;
	
	Sfen::LoadPosition(pos, sfen2);

	int i = 0;

	for (auto& str: moves)
	{
		Move movedata2 = Sfen::ParseMove(pos, str);

		if (is_move(movedata2.move_type()))
		{
			ASSERT_EQ(MoveCheck::IsValid(pos, movedata2), true);

			pos.DoMove(movedata2);
		}
		i++;
	}

	Sfen::LoadPosition(pos, sfen2);

	ASSERT_EQ(MoveCheck::IsValid(pos, Move(MoveType::PASS)), true);

	Move movedata(MoveType::NORMAL);

	ASSERT_EQ(MoveCheck::IsValid(pos, movedata), false);
	movedata.set_to(Square::SQ_68);
	ASSERT_EQ(MoveCheck::IsValid(pos, movedata), false);
	movedata.set_piece(Piece::W_SILVER);
	movedata.set_side(WHITE);
	ASSERT_EQ(MoveCheck::IsValid(pos, movedata), false);
	movedata.set_move_type(MoveType::DROP);
	ASSERT_EQ(MoveCheck::IsValid(pos, movedata), true);
	movedata.set_piece(Piece::B_SILVER);
	ASSERT_EQ(MoveCheck::IsValid(pos, movedata), false);

	ASSERT_EQ(MoveCheck::IsValid(pos, Move(MoveType::DROP, SQ_NONE, Square::SQ_36, Piece::W_GOLD, Piece::NO_PIECE)), false);
	ASSERT_EQ(MoveCheck::IsValid(pos, Move(MoveType::DROP, SQ_NONE, Square::SQ_37, Piece::W_SILVER, Piece::NO_PIECE)), true);
	ASSERT_EQ(MoveCheck::IsValid(pos, Move(MoveType::NORMAL, Square::SQ_81, Square::SQ_73, Piece::W_KNIGHT, Piece::NO_PIECE)), false);

	// Drop‰Â”Û”»’è
	Sfen::LoadPosition(pos, sfen3);
	ASSERT_EQ(MoveCheck::IsValid(pos, Move(MoveType::DROP, SQ_NONE, Square::SQ_99, Piece::W_PAWN, Piece::NO_PIECE)), false);
	ASSERT_EQ(MoveCheck::IsValid(pos, Move(MoveType::DROP, SQ_NONE, Square::SQ_28, Piece::W_PAWN, Piece::NO_PIECE)), true);

	ASSERT_EQ(MoveCheck::IsValid(pos, Move(MoveType::DROP, SQ_NONE, Square::SQ_69, Piece::W_LANCE, Piece::NO_PIECE)), false);
	ASSERT_EQ(MoveCheck::IsValid(pos, Move(MoveType::DROP, SQ_NONE, Square::SQ_38, Piece::W_LANCE, Piece::NO_PIECE)), true);

	ASSERT_EQ(MoveCheck::IsValid(pos, Move(MoveType::DROP, SQ_NONE, Square::SQ_89, Piece::W_KNIGHT, Piece::NO_PIECE)), false);
	ASSERT_EQ(MoveCheck::IsValid(pos, Move(MoveType::DROP, SQ_NONE, Square::SQ_48, Piece::W_KNIGHT, Piece::NO_PIECE)), false);
	ASSERT_EQ(MoveCheck::IsValid(pos, Move(MoveType::DROP, SQ_NONE, Square::SQ_77, Piece::W_KNIGHT, Piece::NO_PIECE)), true);
	// ‘Å‚¿•à‹l‚ß”»’è

	ASSERT_EQ(MoveCheck::IsValid(pos, Move(MoveType::DROP, SQ_NONE, Square::SQ_77, Piece::W_PAWN, Piece::NO_PIECE)), false);
	ASSERT_EQ(MoveCheck::IsValid(pos, Move(MoveType::DROP, SQ_NONE, Square::SQ_77, Piece::W_LANCE, Piece::NO_PIECE)), true);

	// Drop‰Â”Û”»’è
	Sfen::LoadPosition(pos, sfen4);
	ASSERT_EQ(MoveCheck::IsValid(pos, Move(MoveType::DROP, SQ_NONE, Square::SQ_11, Piece::B_PAWN, Piece::NO_PIECE)), false);
	ASSERT_EQ(MoveCheck::IsValid(pos, Move(MoveType::DROP, SQ_NONE, Square::SQ_82, Piece::B_PAWN, Piece::NO_PIECE)), true);

	ASSERT_EQ(MoveCheck::IsValid(pos, Move(MoveType::DROP, SQ_NONE, Square::SQ_41, Piece::B_LANCE, Piece::NO_PIECE)), false);
	ASSERT_EQ(MoveCheck::IsValid(pos, Move(MoveType::DROP, SQ_NONE, Square::SQ_72, Piece::B_LANCE, Piece::NO_PIECE)), true);

	ASSERT_EQ(MoveCheck::IsValid(pos, Move(MoveType::DROP, SQ_NONE, Square::SQ_21, Piece::B_KNIGHT, Piece::NO_PIECE)), false);
	ASSERT_EQ(MoveCheck::IsValid(pos, Move(MoveType::DROP, SQ_NONE, Square::SQ_62, Piece::B_KNIGHT, Piece::NO_PIECE)), false);
	ASSERT_EQ(MoveCheck::IsValid(pos, Move(MoveType::DROP, SQ_NONE, Square::SQ_33, Piece::B_KNIGHT, Piece::NO_PIECE)), true);
	// ‘Å‚¿•à‹l‚ß”»’è

	ASSERT_EQ(MoveCheck::IsValid(pos, Move(MoveType::DROP, SQ_NONE, Square::SQ_33, Piece::B_PAWN, Piece::NO_PIECE)), false);
	ASSERT_EQ(MoveCheck::IsValid(pos, Move(MoveType::DROP, SQ_NONE, Square::SQ_33, Piece::B_LANCE, Piece::NO_PIECE)), true);


}

// [TestMethod, Description("")]
TEST_F(MoveCheckTest, testCanPromota)
{
	Position pos;
	
	Sfen::LoadPosition(pos, sfen2);

	int i = 0;

	for (auto str : moves)
	{
		Move movedata = Sfen::ParseMove(pos, str);
		ASSERT_EQ(MoveCheck::CanPromota(movedata), promotions[i]);

		if (is_move(movedata.move_type()))
		{
			pos.DoMove(movedata);
		}
		i++;
	}


}

// [TestMethod, Description("")]
TEST_F(MoveCheckTest, testForcePromotion)
{

	ASSERT_EQ(MoveCheck::ForcePromotion(Piece::B_PAWN, Square::SQ_91), true);
	ASSERT_EQ(MoveCheck::ForcePromotion(Piece::B_PAWN, Square::SQ_92), false);

	ASSERT_EQ(MoveCheck::ForcePromotion(Piece::B_LANCE, Square::SQ_31), true);
	ASSERT_EQ(MoveCheck::ForcePromotion(Piece::B_LANCE, Square::SQ_32), false);

	ASSERT_EQ(MoveCheck::ForcePromotion(Piece::B_KNIGHT, Square::SQ_31), true);
	ASSERT_EQ(MoveCheck::ForcePromotion(Piece::B_KNIGHT, Square::SQ_22), true);
	ASSERT_EQ(MoveCheck::ForcePromotion(Piece::B_KNIGHT, Square::SQ_33), false);

	ASSERT_EQ(MoveCheck::ForcePromotion(Piece::W_PAWN, Square::SQ_89), true);
	ASSERT_EQ(MoveCheck::ForcePromotion(Piece::W_PAWN, Square::SQ_88), false);

	ASSERT_EQ(MoveCheck::ForcePromotion(Piece::W_LANCE, Square::SQ_29), true);
	ASSERT_EQ(MoveCheck::ForcePromotion(Piece::W_LANCE, Square::SQ_28), false);

	ASSERT_EQ(MoveCheck::ForcePromotion(Piece::W_KNIGHT, Square::SQ_59), true);
	ASSERT_EQ(MoveCheck::ForcePromotion(Piece::W_KNIGHT, Square::SQ_48), true);
	ASSERT_EQ(MoveCheck::ForcePromotion(Piece::W_KNIGHT, Square::SQ_67), false);

}

// [TestMethod, Description("")]
TEST_F(MoveCheckTest, testIsCheck)
{

	Position pos;

	Sfen::LoadPosition(pos, sfen);

	Move movedata = Sfen::ParseMove(pos, "7b8b+");

	ASSERT_EQ(MoveCheck::IsCheck(pos), false);

	pos.DoMove(movedata);

	ASSERT_EQ(MoveCheck::IsCheck(pos), true);

	Sfen::LoadPosition(pos, sfen2);
	movedata = Sfen::ParseMove(pos, "S*6h");

	ASSERT_EQ(MoveCheck::IsCheck(pos), false);
	pos.DoMove(movedata);

	ASSERT_EQ(MoveCheck::IsCheck(pos), true);

}


