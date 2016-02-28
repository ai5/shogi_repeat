#include <gtest/gtest.h>

#include <iostream>

#include "CProcess.h"
#include "Semaphore.h"

TEST(ProcessTest, test)
{

	Process proc;

	proc.StartInfo.CreateNoWindow = false;
#ifdef _WIN32
	proc.StartInfo.FileName = "tchild.exe";
#else
	proc.StartInfo.FileName = "./tchild.out";
#endif
	proc.StartInfo.RedirectStandardInput = true;
	proc.StartInfo.RedirectStandardOutput = true;

	Semaphore sem;

	proc.SetExited([&] {
		std::cout << "exited" << std::endl;
		sem.post();
	});

	std::string str;
	ASSERT_EQ(proc.Start(), true);
	proc.StdOut.ReadLine(str);

	ASSERT_EQ(proc.StdIn.Write("usi\n"), 4);


	ASSERT_EQ(proc.StdOut.ReadLine(str), true);
	ASSERT_EQ(str, "id name child dummy");
	ASSERT_EQ(proc.StdOut.ReadLine(str), true);
	ASSERT_EQ(str, "id author child");

	while (proc.StdOut.ReadLine(str))
	{
		if (str == "usiok") break;
	}

	ASSERT_EQ(proc.StdIn.Write("quit\n"), 5);
	
	ASSERT_EQ(proc.StdOut.ReadLine(str), false);

	sem.wait(); // exit‘Ò‚¿

	proc.WaitForExit(-1);

	proc.Close();
	proc.Close(); // <= —]•ª‚ÈŒÄ‚Ño‚µ
}
