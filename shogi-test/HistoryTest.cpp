#include <gtest/gtest.h>
#include <string>

#include "History.h"

#include "Notation.h"
#include "Sfen.h"

class HistoryTest : public ::testing::Test {

protected:
	static std::string sfen1;
	static std::string move;
	static std::string sfen2;
	static std::string sfen3;
};

std::string HistoryTest::sfen1 = "position startpos moves 7g7f 3c3d 2h6h 8b6b 6h2h 6b8b 2h6h 8b6b 6h2h 6b8b 2h6h 8b6b 6h2h";
std::string HistoryTest::move = "6b8b";

std::string HistoryTest::sfen2 = "position sfen lnsgk3l/1r7/pppppp+Bpp/6p2/9/2P6/3PP1PPP/7R1/LNSGKGSNL w GS3Pbn 1 moves 5a4a 3c2c 4a5a 2c3c 5a4a 3c2c 4a5a 2c3c 5a4a 3c2c 4a5a 2c3c 5a4a 3c2c 4a5a 2c3c";
std::string HistoryTest::sfen3 = "position startpos moves 2g2f 8c8d 7g7f 8d8e 2f2e 3c3d 6i7h 4a3b 2e2d 2c2d 2h2d 8e8f 8g8f 8b8f 2d3d 2b3c 3d3f 5a5b P*8g 8f8e 3f2f 3a2b 3i4h 3c8h+ 7i8h B*4d 2f2d 2b2c 2d4d 4c4d B*1f P*2e 8i7g 8e9e 3g3f 2a3c 5i6i 7a7b 7h6h 7c7d 9g9f 9e9d B*8b 7d7e 8b9a+ 8a7c 9a9b 7e7f 7g6e P*8f 6i7h 8f8g+ 8h8g P*8f 8g8f 9d8d 6e5c+ 5b5c 9b9c 8d8f 9c7e N*6d 7e8f S*7g 6h7g 7f7g+ 8f7g P*7f 7g6f G*7g 7h6i R*8h L*5d 5c4c 6f7g 7f7g+ G*5c 4c3d S*3e 3d4e 3e4f 4e3d 4f3e 3d4e 3e4f 4e3d 4f3e 3d4e R*6e 7c6e 3e4f 4e3d 4f3e 3d4e 3e4f 4e3d 4f3e 3d4e 3e4f 4e3d 4f3e 3d4e";

// [TestMethod, Description("ƒeƒXƒg")]
TEST_F(HistoryTest, TestHistoryInit)
{

	Notation notation;
	Sfen::LoadNotation(notation, HistoryTest::sfen1);

	History history;

	history.Init(notation);

	Move movedata = Sfen::ParseMove(notation.position(), HistoryTest::move);

	notation.AddMove(movedata);
	history.Add(notation.position().hashkey(), false);

	ASSERT_EQ(history.IsRepetition(), true);
	ASSERT_EQ(history.IsRepetitionCheck(), false);
	ASSERT_EQ(history.IsRepetitionCheckOpp(), false);

	history.Clear();

	Sfen::LoadNotation(notation, HistoryTest::sfen2);
	history.Init(notation);
	ASSERT_EQ(history.IsRepetition(), true);
	ASSERT_EQ(history.IsRepetitionCheck(), true);
	ASSERT_EQ(history.IsRepetitionCheckOpp(), false);

	Sfen::LoadNotation(notation, HistoryTest::sfen3);
	history.Init(notation);
	ASSERT_EQ(history.IsRepetition(), true);
	ASSERT_EQ(history.IsRepetitionCheck(), false);
	ASSERT_EQ(history.IsRepetitionCheckOpp(), true);
}

