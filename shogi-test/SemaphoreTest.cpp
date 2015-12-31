#include <gtest/gtest.h>

#include <iostream>
#include <thread>

#include "Semaphore.h"


using namespace std;


TEST(SemaphoreTest, test)
{

	Semaphore sem;

	ASSERT_EQ(sem.wait(100), false);

	sem.post();
	ASSERT_EQ(sem.wait(100), true);

	thread th1([&] {

		cout << "post1" << endl;
		sem.post();
		this_thread::sleep_for(chrono::milliseconds(1000));
		cout << "post2" <<endl;
		sem.post();
		cout << "post3" << endl;
		sem.post();

	});

	sem.wait();
	cout << "wait1" << endl;
	sem.wait();
	cout << "wait2" << endl;
	sem.wait();
	cout << "wait3" << endl;


	th1.join();
}

