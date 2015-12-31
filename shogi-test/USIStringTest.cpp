#include <gtest/gtest.h>

#include <iostream>

#include "USIString.h"

TEST(USIStringTest, test)
{
	int num;

	ASSERT_EQ(USIString::ParseNum("100", &num), true);
	ASSERT_EQ(num, 100);

	ASSERT_EQ(USIString::ParseNum("100k", &num), true);
	ASSERT_EQ(num, 100000);

	ASSERT_EQ(USIString::ParseNum("123 ", &num), true);
	ASSERT_EQ(num, 123);

	ASSERT_EQ(USIString::ParseNum("123456789", &num), true);
	ASSERT_EQ(num, 123456789);
}

TEST(USIStringTest, testToknizer)
{
	// option name <id> type <t> default <x>[min <y> max <z>]{ var <v> }
	std::string str = "option name MyName type spin default 10 min 0 max 65536";
	USITokenizer tok(str);

	ASSERT_EQ(tok.GetToken(), "option");
	ASSERT_EQ(tok.GetToken(), "name");
	ASSERT_EQ(tok.GetToken(), "MyName");
	ASSERT_EQ(tok.GetToken(), "type");
	ASSERT_EQ(tok.GetToken(), "spin");
	ASSERT_EQ(tok.GetToken(), "default");
	ASSERT_EQ(tok.GetToken(), "10");
	ASSERT_EQ(tok.GetToken(), "min");
	ASSERT_EQ(tok.GetToken(), "0");
	ASSERT_EQ(tok.GetToken(), "max");
	ASSERT_EQ(tok.GetToken(), "65536");

	str = "option name My Name type string default name str";
	USITokenizer tok2(str);

	ASSERT_EQ(tok2.GetToken(), "option");
	ASSERT_EQ(tok2.GetToken(), "name");
	ASSERT_EQ(tok2.GetTokenName("type"), "My Name");
	ASSERT_EQ(tok2.GetToken(), "type");
	ASSERT_EQ(tok2.GetToken(), "string");
	ASSERT_EQ(tok2.GetToken(), "default");
	ASSERT_EQ(tok2.GetTokenLast(), "name str");



}



