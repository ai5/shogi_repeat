#include <gtest/gtest.h>

#include <types.h>

TEST(FileTest, test)
{
	ASSERT_EQ(File::FILE_1, 8);
	ASSERT_EQ(File::FILE_9, 0);

	ASSERT_EQ(file_from_suji(1), FILE_1);
	ASSERT_EQ(file_from_suji(2), FILE_2);
	ASSERT_EQ(file_from_suji(3), FILE_3);
	ASSERT_EQ(file_from_suji(4), FILE_4);
	ASSERT_EQ(file_from_suji(5), FILE_5);
	ASSERT_EQ(file_from_suji(6), FILE_6);
	ASSERT_EQ(file_from_suji(7), FILE_7);
	ASSERT_EQ(file_from_suji(8), FILE_8);
	ASSERT_EQ(file_from_suji(9), FILE_9);

	ASSERT_EQ(suji_from_file(FILE_1), 1);
	ASSERT_EQ(suji_from_file(FILE_2), 2);
	ASSERT_EQ(suji_from_file(FILE_3), 3);
	ASSERT_EQ(suji_from_file(FILE_4), 4);
	ASSERT_EQ(suji_from_file(FILE_5), 5);
	ASSERT_EQ(suji_from_file(FILE_6), 6);
	ASSERT_EQ(suji_from_file(FILE_7), 7);
	ASSERT_EQ(suji_from_file(FILE_8), 8);
	ASSERT_EQ(suji_from_file(FILE_9), 9);
}
