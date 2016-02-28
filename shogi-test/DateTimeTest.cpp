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
