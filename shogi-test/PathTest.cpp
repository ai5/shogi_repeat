#include <gtest/gtest.h>

#include <iostream>

#include "Path.h"

TEST(PathTest, test)
{
	// ‚±‚ÌƒeƒXƒg‚ÍŠÂ‹«‚É‚æ‚Á‚Ä•Ï‚í‚é
#ifdef _WIN32
	ASSERT_EQ(Path::FullPath("shogi-test.exe"), "I:\\projects\\shogi\\Test\\shogi-test.exe");
	ASSERT_EQ(Path::FolderPath("I:\\projects\\shogi\\aaaa"), "I:\\projects\\shogi");
	ASSERT_EQ(Path::FileName("I:\\projects\\shogi\\Test\\shogi-test.exe"), "shogi-test.exe");
#else
	ASSERT_EQ(Path::FullPath("shogi-test.exe"), "/mnt/hgfs/I/projects/shogi/Test/shogi-test.exe");
	ASSERT_EQ(Path::FolderPath("/I/projects/shogi/aaaa"), "/I/projects/shogi");
	ASSERT_EQ(Path::FileName("/I/projects/shogi/Test/shogi-test.exe"), "shogi-test.exe");
#endif
}
