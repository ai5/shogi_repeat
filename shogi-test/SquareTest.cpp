#include <gtest/gtest.h>

#include <types.h>

TEST(SquareTest, test)
{
	ASSERT_EQ(file_of(Square::SQ_91), FILE_9);
	ASSERT_EQ(file_of(Square::SQ_19), FILE_1);

	ASSERT_EQ(rank_of(Square::SQ_91), RANK_1);
	ASSERT_EQ(rank_of(Square::SQ_19), RANK_9);
}

TEST(SquareTest, testMake)
{
	ASSERT_EQ(make_square(File::FILE_9, Rank::RANK_9), Square::SQ_99);
	ASSERT_EQ(make_square(File::FILE_1, Rank::RANK_1), Square::SQ_11);

	ASSERT_EQ(make_square((File)0, (Rank)0), Square::SQ_91);
	ASSERT_EQ(make_square((File)8, (Rank)8), Square::SQ_19);
}


