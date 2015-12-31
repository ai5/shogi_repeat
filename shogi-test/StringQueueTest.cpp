#include <gtest/gtest.h>

#include <iostream>
#include <thread>

#include "StringQueue.h"


using namespace std;


TEST(StringQueueTest, test)
{

	StringQueue str_que;

	str_que.Push("test");

	std::string str;

	ASSERT_EQ(str_que.Pop(str, 1000), STRING_QUEUE_ERR::OK);
	ASSERT_EQ(str, "test");

	thread th1([&] {

		str_que.Push("test1");
		str_que.Push("test2");
		str_que.Push("test3");

		this_thread::sleep_for(chrono::milliseconds(2000));
		str_que.Push(std::string("test4"));
		str_que.Push(std::string("test5"));
		str_que.Push(nullptr);

	});

	ASSERT_EQ(str_que.Pop(str, 1000), STRING_QUEUE_ERR::OK);
	ASSERT_EQ(str, "test1");

	ASSERT_EQ(str_que.Pop(str, 1000), STRING_QUEUE_ERR::OK);
	ASSERT_EQ(str, "test2");
	ASSERT_EQ(str_que.Pop(str, 1000), STRING_QUEUE_ERR::OK);
	ASSERT_EQ(str, "test3");

	ASSERT_EQ(str_que.Pop(str, 1000), STRING_QUEUE_ERR::TIMEOUT);
	ASSERT_EQ(str_que.IsClose(), false);

	ASSERT_EQ(str_que.Pop(str, -1), STRING_QUEUE_ERR::OK);
	ASSERT_EQ(str, "test4");
	ASSERT_EQ(str_que.Pop(str, -1), STRING_QUEUE_ERR::OK);
	ASSERT_EQ(str, "test5");
 	ASSERT_EQ(str_que.Pop(str, -1), STRING_QUEUE_ERR::ERR);


	str_que.Close();
	ASSERT_EQ(str_que.Pop(str, 1000), STRING_QUEUE_ERR::CLOSE);
	ASSERT_EQ(str_que.IsClose(), true);

	th1.join();
}


