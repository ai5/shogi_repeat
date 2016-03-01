#include <gtest/gtest.h>

#include <iostream>
#include "DateTime.h"

TEST(DateTimeTest, test)
{
	DateTime date(2016, 1, 5, 11, 18, 19);

	std::string str = date.ToString();
	ASSERT_EQ(str, "2016/1/5 11:18:19");

	DateTime now = DateTime::Now();

	std::wstring wstr = now.ToWString();
	printf("%ls\n", wstr.c_str());

}

TEST(DateTimeTest, ToTimeStringTest)
{
	ASSERT_EQ(DateTime::ToTimeString(1000), "1");
	ASSERT_EQ(DateTime::ToTimeString(900), "0.9");
	ASSERT_EQ(DateTime::ToTimeString(980), "0.98");
	ASSERT_EQ(DateTime::ToTimeString(987), "0.987");


	ASSERT_EQ(DateTime::ToTimeString(1987), "1.987");
	ASSERT_EQ(DateTime::ToTimeString(32980), "32.98");
	ASSERT_EQ(DateTime::ToTimeString(62900), "1:02.9");

	ASSERT_EQ(DateTime::ToTimeString(100 * 60 * 1000 + 15 * 1000), "100:15");
	ASSERT_EQ(DateTime::ToTimeString(100*60*1000+15*1000+987), "100:15.987");
}