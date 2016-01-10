#include <gtest/gtest.h>

#include "types.h"
#include "Handicap.h"


TEST(HandicapTest, TestIsBlack)
{
	ASSERT_EQ(HandicapExtention::IsBlack(Handicap::HIRATE), true);
	ASSERT_EQ(HandicapExtention::IsBlack(Handicap::OTHER), true);
	ASSERT_EQ(HandicapExtention::IsBlack(Handicap::H2), false); 

}

TEST(HandicapTest, TestToString)
{
	ASSERT_TRUE(HandicapExtention::ToString(Handicap::HIRATE) == "平手");
	ASSERT_TRUE(HandicapExtention::ToString(Handicap::OTHER) == "その他");
	ASSERT_TRUE(HandicapExtention::ToString(Handicap::KYO) == "香落ち");
	ASSERT_TRUE(HandicapExtention::ToString(Handicap::RIGHT_KYO) == "右香落ち");
}

TEST(HandicapTest, TestFromString)
{
	ASSERT_EQ(HandicapExtention::FromString("平手"), Handicap::HIRATE);
	ASSERT_EQ(HandicapExtention::FromString("香落ち"), Handicap::KYO);
	ASSERT_EQ(HandicapExtention::FromString("右香落ち"), Handicap::RIGHT_KYO);
	ASSERT_EQ(HandicapExtention::FromString("角落ち"), Handicap::KAKU);
	ASSERT_EQ(HandicapExtention::FromString("飛車落ち"), Handicap::HISYA);
	ASSERT_EQ(HandicapExtention::FromString("飛香落ち"), Handicap::HIKYO);

	ASSERT_EQ(HandicapExtention::FromString("二枚落ち"), Handicap::H2);
	ASSERT_EQ(HandicapExtention::FromString("三枚落ち"), Handicap::H3);
	ASSERT_EQ(HandicapExtention::FromString("四枚落ち"), Handicap::H4);
	ASSERT_EQ(HandicapExtention::FromString("五枚落ち"), Handicap::H5);

	ASSERT_EQ(HandicapExtention::FromString("左五枚落ち"), Handicap::LEFT5);
	ASSERT_EQ(HandicapExtention::FromString("六枚落ち"), Handicap::H6);
	ASSERT_EQ(HandicapExtention::FromString("八枚落ち"), Handicap::H8);
	ASSERT_EQ(HandicapExtention::FromString("十枚落ち"), Handicap::H10);

	ASSERT_EQ(HandicapExtention::FromString("その他"), Handicap::OTHER);
}