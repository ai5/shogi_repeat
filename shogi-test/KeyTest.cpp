#include <gtest/gtest.h>

#include <Key.h>
#include <Position.h>
#include <Move.h>


TEST(KeyTest, TestKeyInit)
{
	Position pos;

	Key hashkey(pos.side_to_move(), pos.board(), pos.hand());

	ASSERT_EQ(hashkey.boardkey(), Key::InitialBoardKey);

	hashkey = Key(Key::InitialBoardKey, Key::TurnBit, 0);

	ASSERT_EQ(hashkey.boardkey(), Key::InitialBoardKey);

	Key hashkey2 = Key(hashkey);
	ASSERT_EQ(hashkey.boardkey(), hashkey2.boardkey());
	ASSERT_EQ(hashkey.hand_b(), hashkey2.hand_b());
	ASSERT_EQ(hashkey.hand_w(), hashkey2.hand_w());

	ASSERT_EQ(hashkey.Equals(hashkey2), true);
	ASSERT_EQ(hashkey.Equals(Key(0, 0, 0)), false);
}

TEST(KeyTest, TestKeyDoMove)
{
	Position pos;
	Key hashkey = Key(pos.side_to_move(), pos.board(), pos.hand());

	Move movedata = Move(MoveType::NORMAL);

	movedata.set_from(Square::SQ_77);
	movedata.set_to(Square::SQ_76);
	movedata.set_piece(Piece::B_PAWN);

	hashkey.DoMove(movedata);

	ASSERT_EQ(hashkey.boardkey(), 4991264001263919599U);
	ASSERT_EQ(hashkey.hand_b(), Key::TurnBit);
	ASSERT_EQ(hashkey.hand_w(), 0U);

	hashkey.UndoMove(movedata);

	ASSERT_EQ(hashkey.boardkey(), Key::InitialBoardKey);
	ASSERT_EQ(hashkey.hand_b(), 0U);
	ASSERT_EQ(hashkey.hand_w(), 0U);

	Move pass = Move(MoveType::PASS);
	hashkey.DoMove(pass);
	ASSERT_EQ(hashkey.boardkey(), Key::InitialBoardKey);
	ASSERT_EQ(hashkey.hand_b(), 1U);
	ASSERT_EQ(hashkey.hand_w(), 0U);

	hashkey.UndoMove(pass);

	ASSERT_EQ(hashkey.boardkey(), Key::InitialBoardKey);
	ASSERT_EQ(hashkey.hand_b(), 0U);

}
