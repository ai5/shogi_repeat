#include <gtest/gtest.h>

#include <string>
#include <vector>

#include "TestCommon.h"
#include "types.h"

#include "Notation.h"
#include "Sfen.h"


class NotationTest : public ::testing::Test {

protected:
	static const std::string sfen1;
	static const std::vector<std::string> moves;
	static const std::string sfen2;


	virtual void SetUp() {
	}

	// virtual void TearDown() {}

};

const std::string NotationTest::sfen1 = "position startpos moves 7g7f 3c3d 2g2f 4c4d 2f2e 2b3c 3i4h 8b4b 5i6h 5a6b 6h7h 3a3b 5g5f 6b7b 8h7g 4a5b 4h5g 3b4c 7h8h 7b8b 9i9h 4c5d 5g6f 6c6d 6i7h 4d4e 6f5g 5d6e 7g3c+ 2a3c 2e2d 2c2d 2h2d 4e4f 2d2a+ B*4d B*5e 4d5e 5f5e 4f4g+ 2a6a B*7b P*4c 5b4c 6a6d 4g5g 6d6e S*5f 6e6d P*6f 6d6f P*6e 6f7g 4b2b P*2c 2b2c 4i3i 7b4e P*4f 5g6g 4f4e 6g7g 8i7g 6e6f B*6a 6f6g+ S*6b 6g7h 7i7h 7a6b G*7b 8b9b 7b6b S*7i 8h7i G*6h 7i8h 6h7h 8h7h S*8i 7h7i 8i7h+ 7i7h 5f6g+ 7h6g P*6f 6g6f 2c2f S*3f 2f3f 3g3f R*6d B*6e 6d6e 7g6e B*9i S*7g 9i7g+ 6f7g S*6h 7g6h P*6g 6h7g S*6h 7g8f 6h7g 8f7g 9c9d G*8b 9b8b B*7a 8b9b R*7b G*8b 7b8b+ resign";
const std::vector<std::string> NotationTest::moves= { "7b6b", "S*7i", "8h7i", "G*6h", "7i6h", "R*6f", "S*6g", "6f6b", "S*7a", "2c2b", "7a6b+", "2b6b", "G*7b", "S*7a", "7b6b", "G*5g" };
const std::string NotationTest::sfen2 = "position startpos moves 7g7f 3c3d 2g2f 4c4d 2f2e 2b3c 3i4h 8b4b 5i6h 5a6b 6h7h 3a3b 5g5f 6b7b 8h7g 4a5b 4h5g 3b4c 7h8h 7b8b 9i9h 4c5d 5g6f 6c6d 6i7h 4d4e 6f5g 5d6e 7g3c+ 2a3c 2e2d 2c2d 2h2d 4e4f 2d2a+ B*4d B*5e 4d5e 5f5e 4f4g+ 2a6a B*7b P*4c 5b4c 6a6d 4g5g 6d6e S*5f 6e6d P*6f 6d6f P*6e 6f7g 4b2b P*2c 2b2c 4i3i 7b4e P*4f 5g6g 4f4e 6g7g 8i7g 6e6f B*6a 6f6g+ S*6b 6g7h 7i7h 7a6b G*7b 8b9b 7b6b S*7i 8h7i G*6h 7i6h R*6f S*6g 6f6b S*7a 2c2b 7a6b+ 2b6b G*7b S*7a 7b6b G*5g 6h7i 5f6g+ 6a8c+ 9b8c R*8e P*8d 8e8d 8c8d R*8e 8d7d B*8c";


// [TestMethod, Description("")]
TEST_F(NotationTest, testInit)
{

	Notation notation;

	ASSERT_EQ(notation.position().hashkey().boardkey(), Key::InitialBoardKey);
	ASSERT_EQ(notation.position().hashkey().hand_b(), 0U);

	ASSERT_EQ(notation.initial_position().hashkey().boardkey(), Key::InitialBoardKey);
	ASSERT_EQ(notation.initial_position().hashkey().hand_b(), 0U);

	ASSERT_EQ(notation.handicap(), Handicap::HIRATE);
	ASSERT_EQ(notation.black_name(), L"");
	ASSERT_EQ(notation.white_name(), L"");
	// ASSERT_EQ(notation.move_current(), notation.MoveFirst);
	// ASSERT_EQ(notation.MovePrev, notation.MoveFirst);

	// copyのテスト

	Sfen::LoadNotation(notation, sfen1);
	notation.set_black_name(L"黒名前");
	notation.set_white_name(L"白名前");
	notation.set_kifu_info(L"日時", L"2015/10/10");

	Notation copynotation(notation);
	copynotation.Last(); // 最後の指し手に移動

	ASSERT_EQ(copynotation.position().hashkey().boardkey(), 4374722627024287036U);
	ASSERT_EQ(copynotation.position().hashkey().hand_b(), 68419591U);

	ASSERT_EQ(copynotation.initial_position().hashkey().boardkey(), Key::InitialBoardKey);
	ASSERT_EQ(copynotation.initial_position().hashkey().hand_b(), 0U);
	ASSERT_EQ(copynotation.black_name(), L"黒名前");
	ASSERT_EQ(copynotation.white_name(), L"白名前");
	ASSERT_EQ(copynotation.kifu_infos().at(L"日時"), L"2015/10/10");
}

#if 0
// [TestMethod, Description("")]
TEST_F(NotationTest, testInitEdit)
{

	Notation notation;


	Sfen::LoadNotation(notation, sfen1);

	notation.Last(); // 最後の指し手に移動

	ASSERT_EQ(notation.position().hashkey().boardkey(), 4374722627024287036U);
	ASSERT_EQ(notation.position().hashkey().hand_b(), 68419591U);

	ASSERT_EQ(notation.initial_position().hashkey().boardkey(), Key::InitialBoardKey);
	ASSERT_EQ(notation.initial_position().hashkey().hand_b(), 0U);

	notation.InitEdit();

	ASSERT_EQ(notation.initial_position().hashkey().boardkey(), 4374722627024287036U);
	ASSERT_EQ(notation.initial_position().hashkey().hand_b(), 68419591U);
	ASSERT_EQ(notation.move_current(), notation.MoveFirst);
	ASSERT_EQ(notation.MovePrev, notation.MoveFirst);
}
#endif

// [TestMethod, Description("")]
TEST_F(NotationTest, testContinue)
{
	Notation notation;

	Sfen::LoadNotation(notation, sfen1);

	notation.Last();
	ASSERT_EQ(notation.move_current().Equals(Move(MoveType::RESIGN)), true);
	ASSERT_EQ(notation.move_current().number(), 116);

	notation.Continue(false);
	ASSERT_EQ(notation.move_current().Equals(Move(MoveType::RESIGN)), false);
	ASSERT_EQ(notation.move_current().number(), 115);

	notation.Init();
	notation.AddMove(MoveKif(MoveType::STOP));
	ASSERT_EQ(notation.move_current().Equals(Move(MoveType::STOP)), true);

	notation.Continue(true);

	ASSERT_EQ(notation.move_current().Equals( Move(MoveType::STOP)), false);
}

// [TestMethod, Description("")]
TEST_F(NotationTest, testSetInitialPosition)
{

	Notation notation;
	Position pos;

	pos.SetHandicapKaku();

	notation.SetInitialPosition(pos);

	ASSERT_EQ(notation.position().hashkey().boardkey(), pos.hashkey().boardkey());
	ASSERT_EQ(notation.position().hashkey().hand_b(), pos.hashkey().hand_b());
}

// [TestMethod, Description("")]
TEST_F(NotationTest, testAddMove)
{
	Notation notation;

	MoveKif movedata(MoveType::NORMAL);
	movedata.set_from(Square::SQ_77);
	movedata.set_to(Square::SQ_76);
	movedata.set_piece(Piece::B_PAWN);

	notation.AddMove(movedata);

	movedata.set_from(Square::SQ_33);
	movedata.set_to(Square::SQ_34);
	movedata.set_piece(Piece::W_PAWN);
	movedata.set_number(1);

	ASSERT_EQ(notation.AddMove(movedata), false);

	movedata.set_number(2);
	ASSERT_EQ(notation.AddMove(movedata), true);

	ASSERT_EQ(notation.AddMove(MoveKif(MoveType::PASS)), true);
	ASSERT_EQ(notation.AddMove(MoveKif(MoveType::STOP)), true);

	ASSERT_EQ(notation.move_current().number(), 4);
}

// [TestMethod, Description("")]
TEST_F(NotationTest, testJump)
{
	Notation notation;

	Sfen::LoadNotation(notation, sfen1);

	notation.Jump(115);
	ASSERT_EQ(notation.position().hashkey().boardkey(), 4374722627024287036U);
	ASSERT_EQ(notation.position().hashkey().hand_b(), 68419591U);

	notation.Jump(1);
	ASSERT_EQ(notation.position().hashkey().boardkey(), 4991264001263919599U);
	ASSERT_EQ(notation.position().hashkey().hand_b(), 1U);

}

// [TestMethod, Description("")]
TEST_F(NotationTest, testChangeChildCurrent)
{
	Notation notation;

	Sfen::LoadNotation(notation, sfen1);

	notation.Jump(72);
	const MoveKif* movenode = nullptr;

	for (auto& str : NotationTest::moves)
	{
		MoveKif movedata = Sfen::ParseMove(notation.position(), str);
		ASSERT_EQ(!movedata.IsNoMove(), true);

		notation.MergeMove(movedata);
		movenode = &notation.move_current();
	}

	// notation.Jump(73);
	// ChangeChildCurrentの仕様がC#版と違うことに注意
	notation.ChangeChildCurrent(0);

	ASSERT_EQ(notation.move_current().getCurrentBranchNumber(), 0);

	notation.ChangeChildCurrent(-1);
	ASSERT_EQ(notation.move_current().getCurrentBranchNumber(), -1);

	ASSERT_EQ(movenode != nullptr, true);
#if 0
	notation.ChangeCurrent(movenode);

	// マージテスト
	notation.ChangeCurrent(72);
	ASSERT_EQ(notation.move_current().Children.Count, 2);
	notation.Merge(notation.move_current(), 0, 1);

	notation.ChangeCurrent(72);
	ASSERT_EQ(notation.move_current().Children.Count, 1);
#endif
}

// [TestMethod, Description("")]
TEST_F(NotationTest, testRemove)
{
#if 0
	Notation notation;

	Sfen::LoadNotation(notation, sfen1);

	notation.ChangeCurrent(73);
	MoveNode node = notation.MoveCurrent;

	ASSERT_EQ(notation.Remove(node), true);
	ASSERT_EQ(notation.Remove(node), false);
#endif
}

TEST_F(NotationTest, testMerge)
{
#if 0
	Notation notation;
	Notation notation2;

	Sfen::LoadNotation(notation, sfen1);
	Sfen::LoadNotation(notation2, sfen2);

	notation.Merge(notation2);

	notation.ChangeCurrent(73);
	ASSERT_EQ(notation.move_current().Children.Count, 1);

	notation.ChangeCurrent(74);
	ASSERT_EQ(notation.move_current().Children.Count, 1);

	notation.ChangeCurrent(75);
	ASSERT_EQ(notation.move_current().Children.Count, 1);

	notation.ChangeCurrent(76);
	ASSERT_EQ(notation.move_current().Children.Count, 2);
#endif
}

// [TestMethod, Description("")]
TEST_F(NotationTest, testAddBranch)
{
#if 0
	Notation notation;
	Notation notation2;

	Sfen::LoadNotation(notation, sfen1);
	Sfen::LoadNotation(notation2, sfen2);

	notation.AddBranch(notation2);

	notation.Jump(0);
	ASSERT_EQ(notation.move_current().branches.size(), 1);
#endif
}

TEST_F(NotationTest, testDeleteNotCurrent)
{

	Notation notation;
	Notation notation2;

	Sfen::LoadNotation(notation, sfen1);
	Sfen::LoadNotation(notation2, sfen2);

	notation.Jump(0);
	notation.AddBranch(notation2.moves());

	ASSERT_EQ(notation.move_current().branches().size(), 1);

	notation.DeleteNotCurrent();
	ASSERT_EQ(notation.move_current().branches().size(), 0);

}

// [TestMethod, Description("")]
TEST_F(NotationTest, testMatta)
{
#if 0
	Notation notation;

	Sfen::LoadNotation(notation, sfen1);

	notation.Last();
	ASSERT_EQ(notation.move_current().number(), 116);

	notation.Matta();
	ASSERT_EQ(notation.move_current().number(), 114);

	notation.Matta();
	ASSERT_EQ(notation.move_current().number(), 112);
#endif
}



// [TestMethod, Description("")]
TEST_F(NotationTest, testNext)
{
	Notation notation;
	Sfen::LoadNotation(notation, sfen1);

	notation.First();
	for (int i = 0; i <= 116; i++)
	{
		ASSERT_EQ(notation.move_current().number(), i);
		notation.Next(1);
	}

	ASSERT_EQ(notation.position().hashkey().boardkey(), 4374722627024287036U);
	ASSERT_EQ(notation.position().hashkey().hand_b(), 68419591U);

	// prev
	for (int i = 116; i >= 0; i--)
	{
		ASSERT_EQ(notation.move_current().number(), i);
		notation.Prev(1);
	}

	// ASSERT_EQ(notation.move_current(), notation.MoveFirst);
}

// [TestMethod, Description("")]
TEST_F(NotationTest, testFirst)
{
	Notation notation;
	Sfen::LoadNotation(notation, sfen1);

	notation.Last();
	ASSERT_EQ(notation.move_current().Equals(Move(MoveType::RESIGN)), true);
	ASSERT_EQ(notation.move_current().number(), 116);

	notation.First();

	ASSERT_EQ(notation.move_current().number(), 0);
	// ASSERT_EQ(notation.move_current(), notation.MoveFirst);
}

// [TestMethod, Description("")]
TEST_F(NotationTest, testLast)
{
	Notation notation;
	Sfen::LoadNotation(notation, sfen1);

	notation.First();
	notation.Last();
	ASSERT_EQ(notation.move_current().Equals(Move(MoveType::RESIGN)), true);
	ASSERT_EQ(notation.move_current().number(), 116);

}

// [TestMethod, Description("")]
TEST_F(NotationTest, testset_kifu_info)
{

	Notation notation;

	notation.set_kifu_info(L"日時", L"2015/10/10");
	notation.set_kifu_info(L"場所", L"ここ");

	ASSERT_EQ(notation.kifu_infos().at(L"日時"), L"2015/10/10");
	ASSERT_EQ(notation.kifu_infos().at(L"場所"), L"ここ");
}

TEST_F(NotationTest, testFindMoveNode)
{
#if 0
	Notation notation;
	Sfen::LoadNotation(notation, sfen1);

	notation.Last();
	MoveKif node = notation.move_current();
	ASSERT_EQ(notation.FindMoveNode(node), node);
	ASSERT_EQ(notation.HasMoveNode(node), true);

	notation.First();
	ASSERT_EQ(notation.FindMoveNode(node), node);
	ASSERT_EQ(notation.HasMoveNode(node), true);

	ASSERT_EQ(notation.HasMoveNode(new MoveNode(new MoveKif(MoveType::Stop))), false);
	ASSERT_EQ(notation.FindMoveNode(new MoveNode(new MoveKif(MoveType::Stop))), null);

	ASSERT_EQ(notation.FindMoveNode(new HashKey(4374722627024287036L, 68419591)) != null, true);
#endif
}

// [TestMethod, Description("")]
TEST_F(NotationTest, testSetHandicap)
{
	Notation notation;

	notation.set_handicap(Handicap::RIGHT_KYO);
	ASSERT_EQ(notation.initial_position().hashkey().boardkey(), 0xF48357932EC6B535);
	ASSERT_EQ(notation.initial_position().hashkey().hand_b(), 1U);

	notation.set_handicap(Handicap::KYO);
	ASSERT_EQ(notation.initial_position().hashkey().boardkey(), 0xA76108E09C59222A);
	ASSERT_EQ(notation.initial_position().hashkey().hand_b(), 1U);

	notation.set_handicap(Handicap::KAKU);
	ASSERT_EQ(notation.initial_position().hashkey().boardkey(), 1230110347094849136U);
	ASSERT_EQ(notation.initial_position().hashkey().hand_b(), 1U);

	notation.set_handicap(Handicap::HISYA);
	ASSERT_EQ(notation.initial_position().hashkey().boardkey(), 0xBE8AAF037C7D4997);
	ASSERT_EQ(notation.initial_position().hashkey().hand_b(), 1U);


	notation.set_handicap(Handicap::HIKYO);
	ASSERT_EQ(notation.initial_position().hashkey().boardkey(), 2441288327435227345U);
	ASSERT_EQ(notation.initial_position().hashkey().hand_b(), 1U);

	notation.set_handicap(Handicap::H2);
	ASSERT_EQ(notation.initial_position().hashkey().boardkey(), 0x979200FBE252448B);
	ASSERT_EQ(notation.initial_position().hashkey().hand_b(), 1U);

	notation.set_handicap(Handicap::H3);
	ASSERT_EQ(notation.initial_position().hashkey().boardkey(), 6565054495035034322U);
	ASSERT_EQ(notation.initial_position().hashkey().hand_b(), 1U);

	notation.set_handicap(Handicap::H4);
	ASSERT_EQ(notation.initial_position().hashkey().boardkey(), 0xC4705F8850CDD394);
	ASSERT_EQ(notation.initial_position().hashkey().hand_b(), 1U);

	notation.set_handicap(Handicap::H5);
	ASSERT_EQ(notation.initial_position().hashkey().boardkey(), 6783499617761427302U);
	ASSERT_EQ(notation.initial_position().hashkey().hand_b(), 1U);

	notation.set_handicap(Handicap::LEFT5);
	ASSERT_EQ(notation.initial_position().hashkey().boardkey(), 0xB095236910A1C2F3);
	ASSERT_EQ(notation.initial_position().hashkey().hand_b(), 1U);

	notation.set_handicap(Handicap::H6);
	ASSERT_EQ(notation.initial_position().hashkey().boardkey(), 3082335813779254785U);
	ASSERT_EQ(notation.initial_position().hashkey().hand_b(), 1U);

	notation.set_handicap(Handicap::H8);
	ASSERT_EQ(notation.initial_position().hashkey().boardkey(), 0xC026944BA1385239);
	ASSERT_EQ(notation.initial_position().hashkey().hand_b(), 1U);

	notation.set_handicap(Handicap::H10);
	ASSERT_EQ(notation.initial_position().hashkey().boardkey(), 3965985277409873510U);
	ASSERT_EQ(notation.initial_position().hashkey().hand_b(), 1U);
}

// [TestMethod, Description("")]
TEST_F(NotationTest, testDecisionHandicap)
{
	Notation notation;

	Position pos;

	pos.SetHandicapKyo();
	notation.SetInitialPosition(pos);
	notation.DecisionHandicap();
	ASSERT_EQ(notation.handicap(), Handicap::KYO);

	pos.SetHandicapRightKyo();
	notation.SetInitialPosition(pos);
	notation.DecisionHandicap();
	ASSERT_EQ(notation.handicap(), Handicap::RIGHT_KYO);

	pos.SetHandicapKaku();
	notation.SetInitialPosition(pos);
	notation.DecisionHandicap();
	ASSERT_EQ(notation.handicap(), Handicap::KAKU);

	pos.SetHandicapHisya();
	notation.SetInitialPosition(pos);
	notation.DecisionHandicap();
	ASSERT_EQ(notation.handicap(), Handicap::HISYA);

	pos.SetHandicapHiKyo();
	notation.SetInitialPosition(pos);
	notation.DecisionHandicap();
	ASSERT_EQ(notation.handicap(), Handicap::HIKYO);

	pos.SetHandicap2();
	notation.SetInitialPosition(pos);
	notation.DecisionHandicap();
	ASSERT_EQ(notation.handicap(), Handicap::H2);

	pos.SetHandicap3();
	notation.SetInitialPosition(pos);
	notation.DecisionHandicap();
	ASSERT_EQ(notation.handicap(), Handicap::H3);

	pos.SetHandicap4();
	notation.SetInitialPosition(pos);
	notation.DecisionHandicap();
	ASSERT_EQ(notation.handicap(), Handicap::H4);

	pos.SetHandicap5();
	notation.SetInitialPosition(pos);
	notation.DecisionHandicap();
	ASSERT_EQ(notation.handicap(), Handicap::H5);

	pos.SetHandicapLeft5();
	notation.SetInitialPosition(pos);
	notation.DecisionHandicap();
	ASSERT_EQ(notation.handicap(), Handicap::LEFT5);

	pos.SetHandicap6();
	notation.SetInitialPosition(pos);
	notation.DecisionHandicap();
	ASSERT_EQ(notation.handicap(), Handicap::H6);

	pos.SetHandicap8();
	notation.SetInitialPosition(pos);
	notation.DecisionHandicap();
	ASSERT_EQ(notation.handicap(), Handicap::H8);

	pos.SetHandicap10();
	notation.SetInitialPosition(pos);
	notation.DecisionHandicap();
	ASSERT_EQ(notation.handicap(), Handicap::H10);
}

TEST_F(NotationTest, testBack)
{
	Notation notation;
	Sfen::LoadNotation(notation, sfen1);

	notation.Last();
	ASSERT_EQ(notation.move_current().Equals( Move(MoveType::RESIGN) ), true);
	ASSERT_EQ(notation.move_current().number(), 116);

	notation.Back();
	ASSERT_EQ(is_move(notation.move_current().move_type()), true);
	ASSERT_EQ(notation.move_current().number(), 115);

	notation.Last();
	ASSERT_EQ(is_move(notation.move_current().move_type()), true);
	ASSERT_EQ(notation.move_current().number(), 115);

}