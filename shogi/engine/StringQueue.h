#pragma once

#ifndef SHOGI_STRING_QUEUE_H_
#define SHOGI_STRING_QUEUE_H_

#include <deque>
#include <mutex>
#include <memory>

#include "Semaphore.h"

enum struct STRING_QUEUE_ERR {
	OK,
	ERR,
	TIMEOUT,
	CLOSE,
};

class StringQueue
{
	std::deque<std::unique_ptr<std::string>> queue_;
	std::mutex mtx_;
	Semaphore sem_;

	bool close_ = false;

public:
	StringQueue();
	~StringQueue();

	void Push(const std::string& str);
	void Push(const char* str);
	STRING_QUEUE_ERR Pop(std::string& str, int timeout);


	void Close();
	bool IsClose() { return this->close_; }
};


#endif
