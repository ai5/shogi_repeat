#include <gtest/gtest.h>


#include "TestCommon.h"
#include "types.h"
#include "Kif.h"
#include "Sfen.h"
#include "Notation.h"
#include "StringUtil.h"

class KifTest : public ::testing::Test, public Kif {

protected:
	static const std::string KifuPath;
	static const std::string fileName;
	static const std::string sfen1;

	virtual void SetUp() {
	}

	// virtual void TearDown() {}

};


const std::string KifTest::KifuPath = RootPath;
const std::string KifTest::fileName = "assets/test.kif";
const std::string KifTest::sfen1 = "position startpos moves 7g7f 3c3d 2g2f 4c4d 2f2e 2b3c 3i4h 8b4b 5i6h 5a6b 6h7h 3a3b 5g5f 6b7b 8h7g 4a5b 4h5g 3b4c 7h8h 7b8b 9i9h 4c5d 5g6f 6c6d 6i7h 4d4e 6f5g 5d6e 7g3c+ 2a3c 2e2d 2c2d 2h2d 4e4f 2d2a+ B*4d B*5e 4d5e 5f5e 4f4g+ 2a6a B*7b P*4c 5b4c 6a6d 4g5g 6d6e S*5f 6e6d P*6f 6d6f P*6e 6f7g 4b2b P*2c 2b2c 4i3i 7b4e P*4f 5g6g 4f4e 6g7g 8i7g 6e6f B*6a 6f6g+ S*6b 6g7h 7i7h 7a6b G*7b 8b9b 7b6b S*7i 8h7i G*6h 7i8h 6h7h 8h7h S*8i 7h7i 8i7h+ 7i7h 5f6g+ 7h6g P*6f 6g6f 2c2f S*3f 2f3f 3g3f R*6d B*6e 6d6e 7g6e B*9i S*7g 9i7g+ 6f7g S*6h 7g6h P*6g 6h7g S*6h 7g8f 6h7g 8f7g 9c9d G*8b 9b8b B*7a 8b9b R*7b G*8b 7b8b+ resign";


TEST_F(KifTest, test)
{
	Notation notation;

	Sfen::LoadNotation(notation, sfen1);

	Kif kif;

	kif.Save(notation, "test.kif");
}

TEST_F(KifTest, testLoad)
{
	Notation notation;

	Kif kif;

	kif.Load(notation, KifuPath + "assets/test.kif");

	notation.Last();
	ASSERT_EQ(notation.position().hashkey().boardkey(), 4374722627024287036);
	ASSERT_EQ(notation.position().hashkey().hand_b(), 68419591U);

	ASSERT_EQ(notation.move_current().number(), 116);
	ASSERT_EQ(notation.black_name(), L"斎藤慎太郎五段");
	ASSERT_EQ(notation.white_name(), L"Apery");
	ASSERT_EQ(notation.kifu_infos().at(L"開始日時"), L"2015/3/14");

	ASSERT_EQ(notation.winner(), BLACK);
	ASSERT_EQ(notation.result(), MoveType::RESIGN);

	Notation notation2;
	kif.Load(notation2, KifuPath + "assets/test.bod");
	ASSERT_EQ(notation2.position().hashkey().boardkey(), 4374722627024287036);
	ASSERT_EQ(notation2.position().hashkey().hand_b(), 68419591U);

	ASSERT_EQ(notation.position().hashkey().hand_w(), notation2.position().hashkey().hand_w());
}

TEST_F(KifTest, testSave)
{
	std::string folder = ".";

	Notation notation = Notation();

	Kif kif;


	kif.Load(notation, KifuPath + "assets/test.kif");
	notation.Last();
	ASSERT_EQ(notation.position().hashkey().boardkey(), 4374722627024287036);
	ASSERT_EQ(notation.position().hashkey().hand_b(), 68419591U);



	// テンポラリに書き出し
	kif.Save(notation, folder + "/test.kif");

	// テンポラリから読み出し
	kif.Load(notation, folder + "/test.kif");
	notation.Last();

	ASSERT_EQ(notation.position().hashkey().boardkey(), 4374722627024287036);
	ASSERT_EQ(notation.position().hashkey().hand_b(), 68419591U);

	ASSERT_EQ(notation.move_current().number(), 116);
	ASSERT_EQ(notation.black_name(), L"斎藤慎太郎五段");
	ASSERT_EQ(notation.white_name(), L"Apery");
	ASSERT_EQ(notation.kifu_infos().at(L"開始日時"), L"2015/3/14");

	// 棋譜文字列比較
	std::string kifstring = kif.SaveToString(notation);

	std::string str = StringUtil::Load(KifuPath + "assets/test.kif");
	ASSERT_EQ(kifstring, str);

	// 局面文字列比較
	std::string bod = Kif::ToBodString(notation);

	str = StringUtil::Load(KifuPath + "assets/test.bod");

	ASSERT_EQ(bod, str);

}

TEST_F(KifTest, testSplitMove)
{
	std::deque<std::wstring> str_list;

	Kif::SplitMove(str_list, L"1 ７六歩(77)   ( 00:00/00:00:00)");

	ASSERT_EQ(str_list.size(), 4);

	ASSERT_EQ(str_list[0], L"1");
	ASSERT_EQ(str_list[1], L"７六歩(77)");
	ASSERT_EQ(str_list[2], L"00:00");
	ASSERT_EQ(str_list[3], L"00:00:00)");

	Kif::SplitMove(str_list, L" 32 △７六歩打   ( 12:34/56:78:90)");

	ASSERT_EQ(str_list.size(), 4);

	ASSERT_EQ(str_list[0], L"32");
	ASSERT_EQ(str_list[1], L"△７六歩打");
	ASSERT_EQ(str_list[2], L"12:34");
	ASSERT_EQ(str_list[3], L"56:78:90)");

}


TEST_F(KifTest, testNumberFromKanjiNumber)
{
	size_t index = 0;
	const wchar_t* num[] = {
		L"零",
		L"一", L"二", L"三", L"四", L"五", L"六", L"七", L"八", L"九", L"十",
		L"十一", L"十二", L"十三", L"十四", L"十五", L"十六", L"十七", L"十八", L"十九",
	};

	for (int i = 0; i < 20; i++)
	{
		index = 0;
		ASSERT_EQ(Kif::NumberFromKanjiNumber(num[i], &index), i);
	}

	index = 1;
	ASSERT_EQ(Kif::NumberFromKanjiNumber(L" 二", &index), 2);
	ASSERT_EQ(index, 2);

	index = 2;
	ASSERT_EQ(Kif::NumberFromKanjiNumber(L"　 十二", &index), 12);

	ASSERT_EQ(index, 4);
}

TEST_F(KifTest, testPieceFromWStr)
{
	size_t index = 0;
	ASSERT_EQ(Kif::PieceFromWStr(L"成銀", &index), Piece::B_PRO_SILVER);
	ASSERT_EQ(index, 2);

	index = 1;
	ASSERT_EQ(Kif::PieceFromWStr(L" 成銀", &index), Piece::B_PRO_SILVER);
	ASSERT_EQ(index, 3);

	index = 2;
	ASSERT_EQ(Kif::PieceFromWStr(L" 　銀", &index), Piece::B_SILVER);
	ASSERT_EQ(index, 3);

	ASSERT_EQ(Kif::PieceFromWStr(L"歩", nullptr), Piece::B_PAWN);
	ASSERT_EQ(Kif::PieceFromWStr(L"香", nullptr), Piece::B_LANCE);
	ASSERT_EQ(Kif::PieceFromWStr(L"桂", nullptr), Piece::B_KNIGHT);
	ASSERT_EQ(Kif::PieceFromWStr(L"銀", nullptr), Piece::B_SILVER);
	ASSERT_EQ(Kif::PieceFromWStr(L"金", nullptr), Piece::B_GOLD);
	ASSERT_EQ(Kif::PieceFromWStr(L"角", nullptr), Piece::B_BISHOP);
	ASSERT_EQ(Kif::PieceFromWStr(L"飛", nullptr), Piece::B_ROOK);
	ASSERT_EQ(Kif::PieceFromWStr(L"玉", nullptr), Piece::B_KING);

	ASSERT_EQ(Kif::PieceFromWStr(L"王", nullptr), Piece::B_KING);

	ASSERT_EQ(Kif::PieceFromWStr(L"と", nullptr), Piece::B_PRO_PAWN);
	ASSERT_EQ(Kif::PieceFromWStr(L"成香", nullptr), Piece::B_PRO_LANCE);
	ASSERT_EQ(Kif::PieceFromWStr(L"杏", nullptr), Piece::B_PRO_LANCE);
	ASSERT_EQ(Kif::PieceFromWStr(L"成桂", nullptr), Piece::B_PRO_KNIGHT);
	ASSERT_EQ(Kif::PieceFromWStr(L"圭", nullptr), Piece::B_PRO_KNIGHT);
	ASSERT_EQ(Kif::PieceFromWStr(L"成銀", nullptr), Piece::B_PRO_SILVER);
	ASSERT_EQ(Kif::PieceFromWStr(L"全", nullptr), Piece::B_PRO_SILVER);
	ASSERT_EQ(Kif::PieceFromWStr(L"馬", nullptr), Piece::B_PRO_BISHOP);
	ASSERT_EQ(Kif::PieceFromWStr(L"龍", nullptr), Piece::B_PRO_ROOK);
	ASSERT_EQ(Kif::PieceFromWStr(L"竜", nullptr), Piece::B_PRO_ROOK);

}

TEST_F(KifTest, testNumberFromWChar)
{
	for (int i = 0; i < 10; i++)
	{
		ASSERT_EQ(NumberFromWChar('0' + i), i);
	}

	for (int i = 0; i < 10; i++)
	{
		ASSERT_EQ(NumberFromWChar(L'０' + i), i);
	}

	ASSERT_EQ(NumberFromWChar(L'零'), 0);
	ASSERT_EQ(NumberFromWChar(L'一'), 1);
	ASSERT_EQ(NumberFromWChar(L'二'), 2);
	ASSERT_EQ(NumberFromWChar(L'三'), 3);
	ASSERT_EQ(NumberFromWChar(L'四'), 4);
	ASSERT_EQ(NumberFromWChar(L'五'), 5);
	ASSERT_EQ(NumberFromWChar(L'六'), 6);
	ASSERT_EQ(NumberFromWChar(L'七'), 7);
	ASSERT_EQ(NumberFromWChar(L'八'), 8);
	ASSERT_EQ(NumberFromWChar(L'九'), 9);

}

TEST_F(KifTest, testParseKifMove)
{
	MoveKif move;
	Notation notation;

	ASSERT_TRUE(Kif::ParseKifMove(notation, L"７六歩(77)",  move));
	ASSERT_EQ(move.to(), SQ_76);
	ASSERT_EQ(move.from(), SQ_77);
	ASSERT_EQ(move.piece(), B_PAWN);
	ASSERT_EQ(move.side(), BLACK);
	ASSERT_EQ(move.move_type(), MoveType::NORMAL);

	ASSERT_TRUE(Kif::ParseKifMove(notation, L"△７六香(77)", move));
	ASSERT_EQ(move.to(), SQ_76);
	ASSERT_EQ(move.from(), SQ_77);
	ASSERT_EQ(move.piece(), W_LANCE);
	ASSERT_EQ(move.side(), WHITE);
	ASSERT_EQ(move.move_type(), MoveType::NORMAL);

	ASSERT_TRUE(Kif::ParseKifMove(notation, L"７六桂打", move));
	ASSERT_EQ(move.to(), SQ_76);
	ASSERT_EQ(move.piece(), B_KNIGHT);
	ASSERT_EQ(move.side(), BLACK);
	ASSERT_EQ(move.move_type(), MoveType::DROP);


	ASSERT_TRUE(Kif::ParseKifMove(notation, L"６五成銀(75)", move));
	ASSERT_EQ(move.to(), SQ_65);
	ASSERT_EQ(move.from(), SQ_75);
	ASSERT_EQ(move.piece(), B_PRO_SILVER);
	ASSERT_EQ(move.side(), BLACK);
	ASSERT_EQ(move.move_type(), MoveType::NORMAL);

	ASSERT_TRUE(Kif::ParseKifMove(notation, L"１八銀成(27)", move));
	ASSERT_EQ(move.to(), SQ_18);
	ASSERT_EQ(move.from(), SQ_27);
	ASSERT_EQ(move.piece(), B_SILVER);
	ASSERT_EQ(move.side(), BLACK);
	ASSERT_EQ(move.move_type(), MoveType(MoveType::NORMAL | MoveType::PROMOTION_FLAG));

	ASSERT_TRUE(Kif::ParseKifMove(notation, L"同金(19)", move));
	// ASSERT_EQ(move.to(), SQ_18);
	ASSERT_EQ(move.from(), SQ_19);
	ASSERT_EQ(move.piece(), B_GOLD);
	ASSERT_EQ(move.move_type(), MoveType(MoveType::NORMAL));

	ASSERT_TRUE(Kif::ParseKifMove(notation, L"同 角(98)", move));
	// ASSERT_EQ(move.to(), SQ_18);
	ASSERT_EQ(move.from(), SQ_98);
	ASSERT_EQ(move.piece(), B_BISHOP);
	ASSERT_EQ(move.move_type(), MoveType(MoveType::NORMAL));

	ASSERT_TRUE(Kif::ParseKifMove(notation, L"同　飛成(11)", move));
	// ASSERT_EQ(move.to(), SQ_18);
	ASSERT_EQ(move.from(), SQ_11);
	ASSERT_EQ(move.piece(), B_ROOK);
	ASSERT_EQ(move.move_type(), MoveType(NORMAL| PROMOTION_FLAG));
}

