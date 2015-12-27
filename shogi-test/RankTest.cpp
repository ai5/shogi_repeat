#include <gtest/gtest.h>

#include <types.h>

TEST(RankTest, test)
{
	ASSERT_EQ(Rank::RANK_1, 0);
	ASSERT_EQ(Rank::RANK_9, 8);

	ASSERT_EQ(rank_from_dan(1), RANK_1);
	ASSERT_EQ(rank_from_dan(2), RANK_2);
	ASSERT_EQ(rank_from_dan(3), RANK_3);
	ASSERT_EQ(rank_from_dan(4), RANK_4);
	ASSERT_EQ(rank_from_dan(5), RANK_5);
	ASSERT_EQ(rank_from_dan(6), RANK_6);
	ASSERT_EQ(rank_from_dan(7), RANK_7);
	ASSERT_EQ(rank_from_dan(8), RANK_8);
	ASSERT_EQ(rank_from_dan(9), RANK_9);

	ASSERT_EQ(dan_from_rank(RANK_1), 1);
	ASSERT_EQ(dan_from_rank(RANK_2), 2);
	ASSERT_EQ(dan_from_rank(RANK_3), 3);
	ASSERT_EQ(dan_from_rank(RANK_4), 4);
	ASSERT_EQ(dan_from_rank(RANK_5), 5);
	ASSERT_EQ(dan_from_rank(RANK_6), 6);
	ASSERT_EQ(dan_from_rank(RANK_7), 7);
	ASSERT_EQ(dan_from_rank(RANK_8), 8);
	ASSERT_EQ(dan_from_rank(RANK_9), 9);

}

