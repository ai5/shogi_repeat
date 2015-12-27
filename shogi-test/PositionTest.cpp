#include <gtest/gtest.h>

#include "types.h"
#include "Position.h"

#include "Sfen.h"

// [TestMethod, Description("")]
TEST(PositionTest, testInit)
{
	Position pos;

	ASSERT_EQ(pos.hashkey().boardkey(), Key::InitialBoardKey);
	ASSERT_EQ(pos.hashkey().hand_b(), 0U);

	ASSERT_EQ(pos.IsBlack(File::FILE_7, Rank::RANK_7), true);
	ASSERT_EQ(pos.IsWhite(File::FILE_7, Rank::RANK_7), false);
	ASSERT_EQ(pos.IsEmpty(File::FILE_7, Rank::RANK_7), false);

	ASSERT_EQ(pos.IsBlack(File::FILE_1, Rank::RANK_1), false);
	ASSERT_EQ(pos.IsWhite(File::FILE_1, Rank::RANK_1), true);
	ASSERT_EQ(pos.IsEmpty(File::FILE_1, Rank::RANK_1), false);

	ASSERT_EQ(pos.IsEmpty(File::FILE_5, Rank::RANK_5), true);

	ASSERT_EQ(pos.IsHand(Color::BLACK, PieceType::PAWN), false);
	ASSERT_EQ(pos.IsHand(Color::WHITE, PieceType::PAWN), false);

	ASSERT_EQ(pos.side_to_move(), Color::BLACK);

	pos.set_side_to_move(Color::WHITE);
	ASSERT_EQ(pos.side_to_move(), Color::WHITE);
	ASSERT_EQ(pos.hashkey().boardkey(), Key::InitialBoardKey);
	ASSERT_EQ(pos.hashkey().hand_b(), Key::TurnBit);


	ASSERT_EQ(pos.hand()[Color::BLACK][PieceType::PAWN], 0);
	ASSERT_EQ(pos.hand()[Color::WHITE][PieceType::PAWN], 0);

	std::string sfen = "ln1B4l/k2G5/ppp1pgn1p/6p2/4PP3/2PK2P2/PPN5P/L8/6GNL w RBG3S3Prs2p 92";
	/*
	後手の持駒：飛 銀 歩二
	９ ８ ７ ６ ５ ４ ３ ２ １
	+---------------------------+
	|v香v桂 ・ 角 ・ ・ ・ ・v香|一
	|v玉 ・ ・ 金 ・ ・ ・ ・ ・|二
	|v歩v歩v歩 ・v歩v金v桂 ・v歩|三
	| ・ ・ ・ ・ ・ ・v歩 ・ ・|四
	| ・ ・ ・ ・ 歩 歩 ・ ・ ・|五
	| ・ ・ 歩 玉 ・ ・ 歩 ・ ・|六
	| 歩 歩 桂 ・ ・ ・ ・ ・ 歩|七
	| 香 ・ ・ ・ ・ ・ ・ ・ ・|八
	| ・ ・ ・ ・ ・ ・ 金 桂 香|九
	+---------------------------+
	先手の持駒：飛 角 金 銀三 歩三
	後手番
	*/

	Sfen::LoadPosition(pos, sfen);
	ASSERT_EQ(pos.IsBlack(File::FILE_6, Rank::RANK_1), true);
	ASSERT_EQ(pos.IsWhite(File::FILE_3, Rank::RANK_4), true);
	ASSERT_EQ(pos.IsEmpty(File::FILE_5, Rank::RANK_5), false);

	ASSERT_EQ(pos.IsHand(Color::BLACK, PieceType::PAWN), true);
	ASSERT_EQ(pos.IsHand(Color::BLACK, PieceType::LANCE), false);
	ASSERT_EQ(pos.IsHand(Color::BLACK, PieceType::KNIGHT), false);
	ASSERT_EQ(pos.IsHand(Color::BLACK, PieceType::SILVER), true);
	ASSERT_EQ(pos.IsHand(Color::BLACK, PieceType::GOLD), true);
	ASSERT_EQ(pos.IsHand(Color::BLACK, PieceType::BISHOP), true);
	ASSERT_EQ(pos.IsHand(Color::BLACK, PieceType::ROOK), true);
	ASSERT_EQ(pos.IsHand(Color::WHITE, PieceType::PAWN), true);
	ASSERT_EQ(pos.IsHand(Color::WHITE, PieceType::LANCE), false);
	ASSERT_EQ(pos.IsHand(Color::WHITE, PieceType::KNIGHT), false);
	ASSERT_EQ(pos.IsHand(Color::WHITE, PieceType::SILVER), true);
	ASSERT_EQ(pos.IsHand(Color::WHITE, PieceType::BISHOP), false);
	ASSERT_EQ(pos.IsHand(Color::WHITE, PieceType::ROOK), true);

	ASSERT_EQ(pos.side_to_move(), Color::WHITE);

	ASSERT_EQ(pos.hand()[Color::BLACK][PieceType::PAWN], 3);
	ASSERT_EQ(pos.hand()[Color::WHITE][PieceType::PAWN], 2);

	ASSERT_EQ(pos.hashkey().boardkey(), -5539639773955870868L);
	ASSERT_EQ(pos.hashkey().hand_b(), 85131271U);

}

// [TestMethod, Description("")]
TEST(PositionTest, testDoMove)
{
	std::string sfen = "ln1B4l/k2G5/ppp1pgn1p/6p2/4PP3/2PK2P2/PPN5P/L8/6GNL w RBG3S3Prs2p 92";

	Position pos;
	
	Sfen::LoadPosition(pos, sfen);

	Move movedata = Move(MoveType::DROP);
	Move movedummy = Move();

	movedata.set_to(Square::SQ_64);
	movedata.set_piece(Piece::W_ROOK);
	movedata.set_side(Color::WHITE);

	pos.DoMove(movedata);

	// ASSERT_TRUE(pos.MoveLast.Equals(movedata));
	ASSERT_EQ(pos.hashkey().boardkey(), 2288727272718293328L);
	ASSERT_EQ(pos.hashkey().hand_b(), 85131270U);

	pos.UndoMove(movedata);

	// ASSERT_TRUE(pos.MoveLast.Equals(movedummy));

	ASSERT_EQ(pos.hashkey().boardkey(), -5539639773955870868L);
	ASSERT_EQ(pos.hashkey().hand_b(), 85131271U);

	sfen = "lnsB4l/1k1S5/ppp1pgnrp/6p2/4PP3/2P1s4/PPN3P1P/LK+p6/2S3GNL b BGPrg3p 69";
	Sfen::LoadPosition(pos, sfen);

	movedata = Move(MoveType::CAPTURE);
	movedata.set_to(Square::SQ_78);
	movedata.set_from(Square::SQ_79);
	movedata.set_piece(Piece::B_SILVER);
	movedata.set_capture_piece(Piece::W_PRO_PAWN);
	movedata.set_side(Color::BLACK);

	pos.DoMove(movedata);
	ASSERT_EQ(pos.hashkey().boardkey(), 5021737854523374254L);
	ASSERT_EQ(pos.hashkey().hand_b(), 17825797U);
	// ASSERT_TRUE(pos.MoveLast.Equals(movedata));

	pos.UndoMove(movedata);

	// ASSERT_TRUE(pos.MoveLast.Equals(movedummy));
	ASSERT_EQ(pos.hashkey().boardkey(), -1498543269764304972L);
	ASSERT_EQ(pos.hashkey().hand_b(), 17825794U);

	// 初期局面
	pos = Position();

	movedata = Move(MoveType::NORMAL);
	movedata.set_to(Square::SQ_76);
	movedata.set_from(Square::SQ_77);
	movedata.set_piece(Piece::B_PAWN);
	movedata.set_side(Color::BLACK);

	ASSERT_TRUE(pos.DoMove(movedata));
	ASSERT_EQ(pos.hashkey().boardkey(), 4991264001263919599L);
	ASSERT_EQ(pos.hashkey().hand_b(), 1U);

	pos.UndoMove(movedata);

	// ASSERT_TRUE(pos.MoveLast.Equals(movedummy));
	ASSERT_EQ(pos.hashkey().boardkey(), Key::InitialBoardKey);
	ASSERT_EQ(pos.hashkey().hand_b(), 0U);
}

// [TestMethod, Description("")]
TEST(PositionTest, testClone)
{

	std::string sfen = "ln1B4l/k2G5/ppp1pgn1p/6p2/4PP3/2PK2P2/PPN5P/L8/6GNL w RBG3S3Prs2p 92";

	Position pos, pos2;
	
	Sfen::LoadPosition(pos2, sfen);

	pos = Position(pos2);

	ASSERT_EQ(pos.hashkey().boardkey(), -5539639773955870868L);
	ASSERT_EQ(pos.hashkey().hand_b(), 85131271U);
}

// [TestMethod, Description("")]
TEST(PositionTest, testSearchPiece)
{
	std::string sfen = "ln1B4l/k2G5/ppp1pgn1p/6p2/4PP3/2PK2P2/PPN5P/L8/6GNL w RBG3S3Prs2p 92";

	Position pos;

	Sfen::LoadPosition(pos, sfen);

	ASSERT_EQ(pos.SearchPiece(Piece::B_PAWN), Square::SQ_55);
	ASSERT_EQ(pos.SearchPiece(Piece::W_LANCE), Square::SQ_91);
	ASSERT_EQ(pos.SearchPiece(Piece::B_LANCE), Square::SQ_98);

}

// [TestMethod, Description("")]
TEST(PositionTest, testSetHandicapKyo)
{

	Position pos;

	// 香落ち
	pos.SetHandicapKyo();
	ASSERT_EQ(pos.hashkey().boardkey(), -6385813035845869014);
	ASSERT_EQ(pos.hashkey().hand_b(), 1U);

	// 右香落ち
	pos.SetHandicapRightKyo();
	ASSERT_EQ(pos.hashkey().boardkey(), -827721616849455819L);
	ASSERT_EQ(pos.hashkey().hand_b(), 1U);

	// 角落ち
	pos.SetHandicapKaku();
	ASSERT_EQ(pos.hashkey().boardkey(), 1230110347094849136);
	ASSERT_EQ(pos.hashkey().hand_b(), 1U);

	// 飛車落ち
	pos.SetHandicapHisya();
	ASSERT_EQ(pos.hashkey().boardkey(), -4716765230208824937L);
	ASSERT_EQ(pos.hashkey().hand_b(), 1U);

	// 飛 香 落ち
	pos.SetHandicapHiKyo();
	ASSERT_EQ(pos.hashkey().boardkey(), 2441288327435227345L);
	ASSERT_EQ(pos.hashkey().hand_b(), 1U);

	// ２枚落ち
	pos.SetHandicap2();
	ASSERT_EQ(pos.hashkey().boardkey(), -7524950945548843893L);
	ASSERT_EQ(pos.hashkey().hand_b(), 1U);

	// 3枚落ち
	pos.SetHandicap3();
	ASSERT_EQ(pos.hashkey().boardkey(), 6565054495035034322L);
	ASSERT_EQ(pos.hashkey().hand_b(), 1U);

	// 4枚落ち
	pos.SetHandicap4();
	ASSERT_EQ(pos.hashkey().boardkey(), -4291825405808225388L);
	ASSERT_EQ(pos.hashkey().hand_b(), 1U);

	// 5枚落ち
	pos.SetHandicap5();
	ASSERT_EQ(pos.hashkey().boardkey(), 6783499617761427302L);
	ASSERT_EQ(pos.hashkey().hand_b(), 1U);

	// 左5枚落ち
	pos.SetHandicapLeft5();
	ASSERT_EQ(pos.hashkey().boardkey(), ((uint64_t)-5722628817346772237L));
	ASSERT_EQ(pos.hashkey().hand_b(), 1U);

	// 6枚落ち
	pos.SetHandicap6();
	ASSERT_EQ(pos.hashkey().boardkey(), ((uint64_t)3082335813779254785L));
	ASSERT_EQ(pos.hashkey().hand_b(), 1U);

	// 8枚落ち
	pos.SetHandicap8();
	ASSERT_EQ(pos.hashkey().boardkey(), ((uint64_t)-4600826916764102087L));
	ASSERT_EQ(pos.hashkey().hand_b(), 1U);

	// 10枚落ち
	pos.SetHandicap10();
	ASSERT_EQ(pos.hashkey().boardkey(), ((uint64_t)3965985277409873510L));
	ASSERT_EQ(pos.hashkey().hand_b(), 1U);
}

// [TestMethod, Description("")]
TEST(PositionTest, testReverse)
{

	Position pos;
	pos.SetHandicap6();
	ASSERT_EQ(pos.hashkey().boardkey(), ((uint64_t)3082335813779254785L));
	ASSERT_EQ(pos.hashkey().hand_b(), 1U);

	pos.Reverse();
	ASSERT_EQ(pos.hashkey().boardkey(), ((uint64_t)5722452780900645384L));
	ASSERT_EQ(pos.hashkey().hand_b(), 1U);

}

// [TestMethod, Description("")]
TEST(PositionTest, testInitMatePosition)
{
#if 0
	Position pos = Position();

	pos.InitMatePosition();
	ASSERT_EQ(pos.hashkey().boardkey(), ((uint64_t)8603878756980566988L));
	ASSERT_EQ(pos.hashkey().hand_b(), 0U);
#endif
}

// [TestMethod, Description("")]
TEST(PositionTest, testInBoard)
{

	ASSERT_EQ(Position::InBoard(0, 0), true);
	ASSERT_EQ(Position::InBoard(0, 8), true);
	ASSERT_EQ(Position::InBoard(8, 0), true);
	ASSERT_EQ(Position::InBoard(8, 8), true);
	ASSERT_EQ(Position::InBoard(FILE_1, RANK_1), true);

	ASSERT_EQ(Position::InBoard(-1, -1), false);
	ASSERT_EQ(Position::InBoard(0, -1), false);
	ASSERT_EQ(Position::InBoard(-1, 0), false);
	ASSERT_EQ(Position::InBoard(9, 9), false);
	ASSERT_EQ(Position::InBoard(9, 0), false);
	ASSERT_EQ(Position::InBoard(0, 9), false);

}

// [TestMethod, Description("")]
TEST(PositionTest, testEquals)
{

	Position pos;
	Position pos2;

	ASSERT_EQ(pos.Equals(pos2), true);

	pos.SetHandicapHisya();
	ASSERT_EQ(pos.Equals(pos2), false);
	pos2.SetHandicapHisya();
	ASSERT_EQ(pos.Equals(pos2), true);

	std::string sfen = "ln1B4l/k2G5/ppp1pgn1p/6p2/4PP3/2PK2P2/PPN5P/L8/6GNL w RBG3S3Prs2p 92";

	Sfen::LoadPosition(pos, sfen);
	ASSERT_EQ(pos.Equals(pos2), false);

	Sfen::LoadPosition(pos2, sfen);
	ASSERT_EQ(pos.Equals(pos2), true);
}


