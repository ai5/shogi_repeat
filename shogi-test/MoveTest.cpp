#include <gtest/gtest.h>

#include "types.h"
#include "Move.h"

TEST(MoveTest, test)
{
	Move movedata;

	movedata.set_to(Square::SQ_92);
	movedata.set_from(Square::SQ_88);
	movedata.set_piece(Piece::B_PAWN);
	movedata.set_capture_piece(Piece::W_PAWN);
	movedata.set_move_type(MoveType::CAPTURE);
	movedata.set_side(Color::BLACK);

	ASSERT_EQ(movedata.to(), SQ_92);
	ASSERT_EQ(movedata.from(), SQ_88);
	ASSERT_EQ(movedata.piece(), Piece::B_PAWN);
	ASSERT_EQ(movedata.capture_piece(), Piece::W_PAWN);
	ASSERT_EQ(movedata.side(), Color::BLACK);
	ASSERT_EQ(movedata.move_type(), MoveType::CAPTURE);

	Move movedata2;

	movedata2.set_to(Square(12));
	movedata2.set_from(Square(0));
	movedata2.set_piece(Piece::W_PAWN);
	movedata2.set_move_type(MoveType::DROP);
	movedata2.set_side(Color::WHITE);

	ASSERT_EQ(movedata2.to(), Square(12));
	ASSERT_EQ(movedata2.from(), Square(0));
	ASSERT_EQ(movedata2.piece(), Piece::W_PAWN);
	ASSERT_EQ(movedata2.capture_piece(), Piece::NO_PIECE);
	ASSERT_EQ(movedata2.side(), Color::WHITE);
	ASSERT_EQ(movedata2.move_type(), MoveType::DROP);


	Move movedata3 = Move(movedata);

	ASSERT_EQ(movedata.Equals(movedata2), false);
	ASSERT_EQ(movedata.Equals(movedata3), true);

}

TEST(MoveTest, TestMoveKifuInit)
{
	MoveKif movedataex;

	ASSERT_EQ(movedataex.number(), 0);
	ASSERT_EQ(movedataex.time, 0);
	ASSERT_EQ(movedataex.totalTime, 0);
	ASSERT_EQ(movedataex.parent(), nullptr);
	ASSERT_EQ(movedataex.getCurrentBranchNumber(), -1);

	movedataex = MoveKif(MoveType::NORMAL);

	ASSERT_EQ(movedataex.move_type(), MoveType::NORMAL);
	ASSERT_EQ(movedataex.number(), 0);
	ASSERT_EQ(movedataex.time, 0);
	ASSERT_EQ(movedataex.totalTime, 0);
	ASSERT_EQ(movedataex.parent(), nullptr);
	ASSERT_EQ(movedataex.getCurrentBranchNumber(), -1);



	Move movedata;
	movedata.set_to(Square::SQ_22);
	movedata.set_from(Square::SQ_23);
	movedata.set_piece(Piece::B_PAWN);
	movedata.set_capture_piece(Piece::W_PAWN);
	movedata.set_move_type(MoveType::CAPTURE);

	movedataex =  MoveKif(movedata);
	ASSERT_EQ(movedataex.to(), movedata.to());
	ASSERT_EQ(movedataex.piece(), movedata.piece());
	ASSERT_EQ(movedataex.capture_piece(), movedata.capture_piece());
	ASSERT_EQ(movedataex.move_type(), movedata.move_type());

	MoveKif movekif = MoveKif(movedataex);

	ASSERT_EQ(movedataex.to(), movekif.to());
	ASSERT_EQ(movedataex.piece(), movekif.piece());
	ASSERT_EQ(movedataex.capture_piece(), movekif.capture_piece());
	ASSERT_EQ(movedataex.move_type(), movekif.move_type());

	ASSERT_EQ(movedataex.number(), movekif.number());
	ASSERT_EQ(movedataex.time, movekif.time);
	ASSERT_EQ(movedataex.totalTime, movekif.totalTime);
	ASSERT_EQ(movedataex.parent(), movekif.parent());
	ASSERT_EQ(movedataex.getCurrentBranchNumber(), movekif.getCurrentBranchNumber());


}


TEST(MoveTest, TestAddBranch)
{
	MoveKif move;
	MoveKif pass(MoveType::PASS);
	Moves moves;
	Moves moves2;

	ASSERT_EQ(move.branches().size(), 0);

	move.AddBranch(&moves, pass);

	ASSERT_EQ(move.branches().size(), 1);
	ASSERT_EQ(move.parent(), &moves);

	ASSERT_EQ(move.branches()[0][0].Equals(pass), true);

	moves2.emplace_back(MoveType::PASS);
	moves2.emplace_back(MoveType::DRAW);

	move.AddBranch(&moves, moves2);

	ASSERT_EQ(move.branches().size(), 2);
	ASSERT_EQ(move.branches()[1][0].Equals(pass), true);
	ASSERT_EQ(move.branches()[1][1].Equals(MoveKif(MoveType::DRAW)), true);
}

TEST(MoveTest, TestSwapBranch)
{

	MoveKif move;
	MoveKif pass(MoveType::PASS);
	Moves moves;
	Moves moves2;

	move.AddBranch(&moves, pass);

	moves2.emplace_back(MoveType::NORMAL);
	moves2.emplace_back(MoveType::DRAW);

	move.AddBranch(&moves, moves2);

	move.setCurrentBranchNumber(1);
	ASSERT_EQ(move.getCurrentBranchNumber(), 1);

	move.SwapBranch(0, 1);


	ASSERT_EQ(move.branches().size(), 2);

	ASSERT_EQ(move.branches()[1][0].Equals(MoveKif(MoveType::PASS)), true);
	ASSERT_EQ(move.branches()[0][0].Equals(MoveKif(MoveType::NORMAL)), true);
	ASSERT_EQ(move.branches()[0][1].Equals(MoveKif(MoveType::DRAW)), true);

	ASSERT_EQ(move.getCurrentBranchNumber(), 0);

	move.SwapBranch(0, 1);
	ASSERT_EQ(move.branches().size(), 2);

	ASSERT_EQ(move.branches()[0][0].Equals(MoveKif(MoveType::PASS)), true);
	ASSERT_EQ(move.branches()[1][0].Equals(MoveKif(MoveType::NORMAL)), true);
	ASSERT_EQ(move.branches()[1][1].Equals(MoveKif(MoveType::DRAW)), true);

	ASSERT_EQ(move.getCurrentBranchNumber(), 1);

}

TEST(MoveTest, TestDeleteBranch)
{

	MoveKif move;
	MoveKif pass(MoveType::PASS);
	Moves moves;
	Moves moves2;

	move.AddBranch(&moves, pass);

	moves2.emplace_back(MoveType::NORMAL);
	moves2.emplace_back(MoveType::DRAW);

	move.AddBranch(&moves, moves2);

	move.setCurrentBranchNumber(1);
	ASSERT_EQ(move.getCurrentBranchNumber(), 1);

	move.DeleteBranch(0);

	ASSERT_EQ(move.branches().size(), 1);
	ASSERT_EQ(move.branches()[0][0].Equals(MoveKif(MoveType::NORMAL)), true);
	ASSERT_EQ(move.branches()[0][1].Equals(MoveKif(MoveType::DRAW)), true);

	ASSERT_EQ(move.getCurrentBranchNumber(), 0);

	move.DeleteBranch(0);

	ASSERT_EQ(move.branches().size(), 0);
	ASSERT_EQ(move.getCurrentBranchNumber(), -1);

}

TEST(MoveTest, TestSwapParent)
{

	MoveKif move;
	MoveKif pass(MoveType::PASS);
	Moves moves;
	Moves moves2;

	move.AddBranch(&moves, pass);

	moves2.emplace_back(MoveType::NORMAL);
	moves2.emplace_back(MoveType::DRAW);

	move.AddBranch(&moves, moves2);

	move.setCurrentBranchNumber(1);

	ASSERT_EQ(move.getCurrentBranchNumber(), 1);

	Moves moves3;
	moves3.emplace_back(MoveType::DRAW);

	move.SwapParent(0, moves3);

	ASSERT_EQ(move.branches().size(), 2);

	ASSERT_EQ(move.branches()[0][0].Equals(MoveKif(MoveType::DRAW)), true);
	ASSERT_EQ(move.branches()[1][0].Equals(MoveKif(MoveType::NORMAL)), true);
	ASSERT_EQ(move.branches()[1][1].Equals(MoveKif(MoveType::DRAW)), true);


	move.SwapParent(1, moves3);

	ASSERT_EQ(move.branches()[0][0].Equals(MoveKif(MoveType::DRAW)), true);
	ASSERT_EQ(move.branches()[1][0].Equals(MoveKif(MoveType::DRAW)), true);
	ASSERT_EQ(move.branches()[1].size(), 1);
}

TEST(MoveTest, ChangeParent)
{
	Moves moves1;
	Moves moves2;

	MoveKif move;
	move.set_to(Square::SQ_22);
	move.set_from(Square::SQ_23);
	move.set_piece(Piece::B_PAWN);
	move.set_capture_piece(Piece::W_PAWN);
	move.set_move_type(MoveType::CAPTURE);

	moves1.emplace_back(move);
	moves1.emplace_back(MoveType::PASS);
	moves1.emplace_back(MoveType::STOP);

	moves2.emplace_back(MoveType::PASS);
	moves2.emplace_back(MoveType::STOP);

	moves1[0].AddBranch(&moves1, moves2);
	ASSERT_EQ(moves1[0].branches().size(), 1);

	moves1[0].setCurrentBranchNumber(0);
	ASSERT_EQ(moves1[0].getCurrentBranch()->at(0).parent(), &moves1);

	Moves moves3 = moves1;

	MoveKif::ChangeParent(moves3, nullptr);

	ASSERT_EQ(moves3[0].to(), moves1[0].to());
	ASSERT_EQ(moves3[0].from(), moves1[0].from());
	ASSERT_EQ(moves3[0].piece(), moves1[0].piece());
	ASSERT_EQ(moves3[0].capture_piece(), moves1[0].capture_piece());
	ASSERT_EQ(moves3[0].move_type(), moves1[0].move_type());

	ASSERT_EQ(moves3[1].move_type(), moves1[1].move_type());
	ASSERT_EQ(moves3[2].move_type(), moves1[2].move_type());

	ASSERT_EQ(moves3[0].branches().size(), 1);
	ASSERT_EQ(moves3[0].parent() == nullptr, true);

	ASSERT_EQ(moves3[0].getCurrentBranch()->at(0).parent(), &moves3);

}
