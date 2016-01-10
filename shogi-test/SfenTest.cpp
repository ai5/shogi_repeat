#include <gtest/gtest.h>

#include <string>

#include "TestCommon.h"
#include "types.h"
#include "Sfen.h"
#include "Notation.h"
#include "Position.h"
#include "Move.h"


class SfenTest : public ::testing::Test {

protected:
	static const std::string KifuPath;
	static const std::string fileName;
	static const std::string sfen1;
	static const std::string sfen2;
	static const std::string sfen_moves;
	static const std::string sfen3;
	static const std::string sfen3_moves;
	static const std::string move3;

	virtual void SetUp() {
	}

	// virtual void TearDown() {}

};

const std::string SfenTest::KifuPath = RootPath;
const std::string SfenTest::fileName = "assets/test.sfen";
const std::string SfenTest::sfen1 = "position startpos moves 7g7f 3c3d 2g2f 4c4d 2f2e 2b3c 3i4h 8b4b 5i6h 5a6b 6h7h 3a3b 5g5f 6b7b 8h7g 4a5b 4h5g 3b4c 7h8h 7b8b 9i9h 4c5d 5g6f 6c6d 6i7h 4d4e 6f5g 5d6e 7g3c+ 2a3c 2e2d 2c2d 2h2d 4e4f 2d2a+ B*4d B*5e 4d5e 5f5e 4f4g+ 2a6a B*7b P*4c 5b4c 6a6d 4g5g 6d6e S*5f 6e6d P*6f 6d6f P*6e 6f7g 4b2b P*2c 2b2c 4i3i 7b4e P*4f 5g6g 4f4e 6g7g 8i7g 6e6f B*6a 6f6g+ S*6b 6g7h 7i7h 7a6b G*7b 8b9b 7b6b S*7i 8h7i G*6h 7i8h 6h7h 8h7h S*8i 7h7i 8i7h+ 7i7h 5f6g+ 7h6g P*6f 6g6f 2c2f S*3f 2f3f 3g3f R*6d B*6e 6d6e 7g6e B*9i S*7g 9i7g+ 6f7g S*6h 7g6h P*6g 6h7g S*6h 7g8f 6h7g 8f7g 9c9d G*8b 9b8b B*7a 8b9b R*7b G*8b 7b8b+ resign";
const std::string SfenTest::sfen2 = "ln1B4l/k2G5/ppp1pgn1p/6p2/4PP3/2PK2P2/PPN5P/L8/6GNL w RBG3S3Prs2p 92";
const std::string SfenTest::sfen_moves = "7g7f 3c3d 2g2f 4c4d 2f2e 2b3c 3i4h 8b4b 5i6h 5a6b 6h7h 3a3b 5g5f 6b7b 8h7g 4a5b 4h5g 3b4c 7h8h 7b8b 9i9h 4c5d 5g6f 6c6d 6i7h 4d4e 6f5g 5d6e 7g3c+ 2a3c 2e2d 2c2d 2h2d 4e4f 2d2a+ B*4d B*5e 4d5e 5f5e 4f4g+ 2a6a B*7b P*4c 5b4c 6a6d 4g5g 6d6e S*5f 6e6d P*6f 6d6f P*6e 6f7g 4b2b P*2c 2b2c 4i3i 7b4e P*4f 5g6g 4f4e 6g7g 8i7g 6e6f B*6a 6f6g+ S*6b 6g7h 7i7h 7a6b G*7b 8b9b 7b6b S*7i 8h7i G*6h 7i8h 6h7h 8h7h S*8i 7h7i 8i7h+ 7i7h 5f6g+ 7h6g P*6f 6g6f 2c2f S*3f 2f3f 3g3f R*6d B*6e 6d6e 7g6e B*9i S*7g 9i7g+ 6f7g S*6h 7g6h P*6g 6h7g S*6h 7g8f 6h7g 8f7g 9c9d G*8b 9b8b B*7a 8b9b R*7b G*8b 7b8b+";
const std::string SfenTest::sfen3 = "position startpos moves 7g7f 3c3d 2g2f 4c4d 2f2e 2b3c 3i4h";
const std::string SfenTest::sfen3_moves = "7g7f 3c3d 2g2f 4c4d 2f2e 2b3c 3i4h";
const std::string SfenTest::move3 = "8b4b";



TEST_F(SfenTest, testTypeOf)
{
}


TEST_F(SfenTest, testIsSfen)
{

	ASSERT_EQ(Sfen::IsSfen("position"), true);
	ASSERT_EQ(Sfen::IsSfen("sfen"), true);
	ASSERT_EQ(Sfen::IsSfen("startpos"), true);
	ASSERT_EQ(Sfen::IsSfen(""), false);
	ASSERT_EQ(Sfen::IsSfen("abcde"), false);
	ASSERT_EQ(Sfen::IsSfen("moves"), true);

}

// [TestMethod, Description("")]
TEST_F(SfenTest, testLoad)
{
	Notation notation;
	Sfen sfen;

	sfen.Load(notation, KifuPath + fileName);
	ASSERT_EQ(notation.position().hashkey().boardkey(), 4374722627024287036);
	ASSERT_EQ(notation.position().hashkey().hand_b(), 68419591U);

	ASSERT_EQ(notation.number(), 116);

}

// [TestMethod, Description("")]
TEST_F(SfenTest, testLoadNotation)
{

	Notation notation;
	Sfen sfen;

	sfen.LoadFromString(notation, sfen1);


	ASSERT_EQ(notation.position().hashkey().boardkey(), 4374722627024287036);
	ASSERT_EQ(notation.position().hashkey().hand_b(), 68419591U);

}

// [TestMethod, Description("")]
TEST_F(SfenTest, testLoadPosition)
{
	Position pos;
	Sfen::LoadPosition(pos, sfen2);

	ASSERT_EQ(pos.hashkey().boardkey(), -5539639773955870868);
	ASSERT_EQ(pos.hashkey().hand_b(), 85131271U);
}

// [TestMethod, Description("")]
TEST_F(SfenTest, testSave)
{

	std::string folder = ".";
	Sfen sfen;

	Notation notation;

	sfen.LoadFromString(notation, sfen1);
	// テンポラリに書き出し
	sfen.Save(notation, folder + "/test.sfen");

	Notation notation2;

	// テンポラリから読み出し
	sfen.Load(notation2, folder + "/test.sfen");

	// 最終局面のハッシュ値と手数を確認
	ASSERT_EQ(notation.position().hashkey().boardkey(), 4374722627024287036);
	ASSERT_EQ(notation.position().hashkey().hand_b(), 68419591U);
	ASSERT_EQ(notation.number(), 116);
}

// [TestMethod, Description("")]
TEST_F(SfenTest, testLoadFromString)
{

	Notation notation;
	Sfen sfen;

	sfen.LoadFromString(notation, sfen1);
	std::string str = sfen.SaveToString(notation);
	
	ASSERT_EQ(str, sfen1);

}


// [TestMethod, Description("")]
TEST_F(SfenTest, testMovesToString)
{

	Notation notation;
	Sfen sfen;

	sfen.LoadFromString(notation, sfen1);
	std::string str = Sfen::MovesToString(notation);
	ASSERT_EQ(str, sfen_moves);

}

// [TestMethod, Description("")]
TEST_F(SfenTest, testMovesToString1)
{

	Notation notation;
	Sfen sfen;

	sfen.LoadFromString(notation, sfen3);

	Move movedata = Sfen::ParseMove(notation.position(), move3);

	std::string str = Sfen::MovesToString(notation) + " " + Sfen::MoveToString(movedata);
	std::string str2 = sfen3_moves + " " + move3;

	ASSERT_EQ(str, str2);
}

// [TestMethod, Description("")]
TEST_F(SfenTest, testParseMove)
{
	Position pos;

	Move movedata = Sfen::ParseMove(pos, "pass");
	// ASSERT_EQ(movedata != null, true);
	ASSERT_EQ(!movedata.IsNoMove(), true);
	ASSERT_EQ(movedata.Equals( Move(MoveType::PASS)), true);

	movedata = Sfen::ParseMove(pos, "resign");
	ASSERT_EQ(!movedata.IsNoMove(), true);
	ASSERT_EQ(movedata.Equals(Move(MoveType::RESIGN)), true);

	movedata = Sfen::ParseMove(pos, "win");
	ASSERT_EQ(!movedata.IsNoMove(), true);
	ASSERT_EQ(movedata.Equals(Move(MoveType::NYUGYOKU_KACHI)), true);

	movedata = Sfen::ParseMove(pos, "draw");
	ASSERT_EQ(!movedata.IsNoMove(), true);
	ASSERT_EQ(movedata.Equals(Move(MoveType::DRAW)), true);

	movedata = Sfen::ParseMove(pos, "0000");
	ASSERT_EQ(!movedata.IsNoMove(), true);
	ASSERT_EQ(movedata.Equals(Move(MoveType::PASS)), true);
}

// [TestMethod, Description("")]
TEST_F(SfenTest, testPieceTypeFromChar)
{

	ASSERT_EQ(Sfen::ConvPieceTypeFromChar('P'), PieceType::PAWN);
	ASSERT_EQ(Sfen::ConvPieceTypeFromChar('L'), PieceType::LANCE);
	ASSERT_EQ(Sfen::ConvPieceTypeFromChar('N'), PieceType::KNIGHT);
	ASSERT_EQ(Sfen::ConvPieceTypeFromChar('S'), PieceType::SILVER);
	ASSERT_EQ(Sfen::ConvPieceTypeFromChar('G'), PieceType::GOLD);
	ASSERT_EQ(Sfen::ConvPieceTypeFromChar('B'), PieceType::BISHOP);
	ASSERT_EQ(Sfen::ConvPieceTypeFromChar('R'), PieceType::ROOK);
	ASSERT_EQ(Sfen::ConvPieceTypeFromChar('K'), PieceType::KING);


}
