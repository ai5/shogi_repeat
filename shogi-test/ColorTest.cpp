#include <gtest/gtest.h>

#include "types.h"


TEST(ColorTest, testOpp)
{
	ASSERT_EQ(opp(Color::BLACK), Color::WHITE);
	ASSERT_EQ(opp(Color::WHITE), Color::BLACK);
}

TEST(ColorTest, testToChar)
{
	ASSERT_EQ(to_char(Color::BLACK), L'▲');
	ASSERT_EQ(to_char(Color::WHITE), L'△');
}


