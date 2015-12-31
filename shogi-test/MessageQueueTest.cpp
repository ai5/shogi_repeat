#include <gtest/gtest.h>

#include <iostream>

#include "MessageQueue.h"

TEST(MessageQueueTest, test)
{
	MessageQueue que;
	int count = 0;

	que.Send(std::make_shared<MessagePost>([&] {
		std::cout << "timeout" << std::endl;
		count++;
	}));


	std::shared_ptr<Message> msg = que.Receive();

	ASSERT_EQ(msg->ID(), MessageID::Post);

	MessagePost* post = static_cast<MessagePost*>(msg.get());
	
	post->Func();

	ASSERT_EQ(count , 1);

}
