#include <gtest/gtest.h>

#include <iostream>
#include <chrono>

#include "Timer.h"


TEST(TimerTest, test)
{
	Timer<void*> timer;
	volatile int count = 0;

	timer.SetTimeOut([&](void*) {
		count++;
		std::cout << "timeout " << count << std::endl;
	}, nullptr);

	timer.Start(100);
	std::this_thread::sleep_for(std::chrono::milliseconds(200));
	ASSERT_EQ(count, 1);
	
	timer.Stop(); // ˆê‰žŽ~‚ß‚é

	timer.Start(1000);
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	timer.Stop(); // Ž~‚ß‚é

	ASSERT_EQ(count, 1); // ƒJƒEƒ“ƒg‚Í‘‚¦‚Ä‚¢‚È‚¢‚Í‚¸

	timer.Start(100);
	std::this_thread::sleep_for(std::chrono::milliseconds(200));
	ASSERT_EQ(count, 2);

	timer.Close();


	TimePoint tp = Timer<int>::Now();
	


}