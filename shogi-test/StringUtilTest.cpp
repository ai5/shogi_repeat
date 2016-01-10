#include <gtest/gtest.h>

#include "StringUtil.h"

TEST(StringUtilTest, testStartsWith)
{
	std::string str = "今日は天気もよく晴れでした";
	
	ASSERT_EQ(StringUtil::StartsWith(str, "今"), true);

	ASSERT_EQ(StringUtil::StartsWith(str, "今日は"), true);
	ASSERT_EQ(StringUtil::StartsWith(str, "今日は天気もよく"), true);
	ASSERT_EQ(StringUtil::StartsWith(str, "今日は天気もよく晴れでした"), true);

	ASSERT_EQ(StringUtil::StartsWith(str, "日"), false);
	ASSERT_EQ(StringUtil::StartsWith(str, "天気もよく"), false);
	ASSERT_EQ(StringUtil::StartsWith(str, "今日は天気もよく晴れでしたが寒かった"), false);

	std::wstring wstr = L"今日は天気もよく晴れでした";

	ASSERT_EQ(StringUtil::StartsWith(wstr, L"今"), true);

	ASSERT_EQ(StringUtil::StartsWith(wstr, L"今日は"), true);
	ASSERT_EQ(StringUtil::StartsWith(wstr, L"今日は天気もよく"), true);
	ASSERT_EQ(StringUtil::StartsWith(wstr, L"今日は天気もよく晴れでした"), true);

	ASSERT_EQ(StringUtil::StartsWith(wstr, L"日"), false);
	ASSERT_EQ(StringUtil::StartsWith(wstr, L"天気もよく"), false);
	ASSERT_EQ(StringUtil::StartsWith(wstr, L"今日は天気もよく晴れでしたが寒かった"), false);
}

TEST(StringUtilTest, testTrim)
{
	ASSERT_EQ(StringUtil::TrimStart(L"テスト", L" 　"), L"テスト");
	ASSERT_EQ(StringUtil::TrimStart(L" テスト", L" 　"), L"テスト");
	ASSERT_EQ(StringUtil::TrimStart(L"　　テスト", L" 　"), L"テスト");
	ASSERT_EQ(StringUtil::TrimStart(L" 　テスト", L" 　"), L"テスト");
	ASSERT_EQ(StringUtil::TrimStart(L"テスト ", L" 　"), L"テスト ");
	ASSERT_EQ(StringUtil::TrimStart(L"テスト 　", L" 　"), L"テスト 　");

	ASSERT_EQ(StringUtil::Trim(L"テスト", L" 　"), L"テスト");
	ASSERT_EQ(StringUtil::Trim(L" テスト", L" 　"), L"テスト");
	ASSERT_EQ(StringUtil::Trim(L"　　テスト", L" 　"), L"テスト");
	ASSERT_EQ(StringUtil::Trim(L" 　テスト", L" 　"), L"テスト");
	ASSERT_EQ(StringUtil::Trim(L"テスト ", L" 　"), L"テスト");
	ASSERT_EQ(StringUtil::Trim(L"テスト 　", L" 　"), L"テスト");

	ASSERT_EQ(StringUtil::Trim(L"　 テスト 　", L" 　"), L"テスト");
}

TEST(StringUtilTest, testIndexOfAny)
{
	ASSERT_EQ(StringUtil::IndexOfAny(L"テスト:テスト", L":："), 3);
	ASSERT_EQ(StringUtil::IndexOfAny(L"テスト：テスト:", L":："), 3);
	ASSERT_EQ(StringUtil::IndexOfAny(L"テスト:テスト：", L":："), 3);
}

TEST(StringUtilTest, testFormat)
{
	ASSERT_EQ(StringUtil::Format("%d", 1), "1");
	ASSERT_EQ(StringUtil::Format("%3d", 3), "  3");
	ASSERT_EQ(StringUtil::Format("%02d", 2), "02");
	ASSERT_EQ(StringUtil::Format("%s", "test"), "test");
	ASSERT_EQ(StringUtil::Format("%6s", "test"), "  test");
	ASSERT_EQ(StringUtil::Format("%-6s", "test"), "test  ");
}

TEST(StringUtilTest, testConvert)
{
	ASSERT_EQ(StringUtil::ConvertWStringFromString("テスト"), L"テスト");
	ASSERT_EQ(StringUtil::ConvertWStringFromString("表示"), L"表示");

	ASSERT_EQ(StringUtil::ConvertStringFromWString(L"テスト"), "テスト");
	ASSERT_EQ(StringUtil::ConvertStringFromWString(L"表示"), "表示");
}

TEST(StringUtilTest, testParseTime)
{
	ASSERT_EQ(StringUtil::ParseTime(L"1:05"), 65);
	ASSERT_EQ(StringUtil::ParseTime(L"1 : 5"), 65);

	ASSERT_EQ(StringUtil::ParseTime(L" 21:05 "), 21*60+5);
	ASSERT_EQ(StringUtil::ParseTime(L"12:34:56"), 12*3600+34*60+56);

}
