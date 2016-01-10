#ifndef LOCKED_QUEUE_H_
#define LOCKED_QUEUE_H_

// Copyright (c) 2012-2013
// Akira Takahashi, Toshihiko Ando, Kohsuke Yuasa,
// Yusuke Ichinohe, Masaya Kusuda, wraith.
// Released under the CC0 1.0 Universal license.
#include <chrono>
#include <iostream>
#include <deque>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional> // ref

template <class T>
struct LockedQueue {
	explicit LockedQueue(int capacity)
		: capacity(capacity)
	{}

	void enqueue(const T& x) {
		std::unique_lock<std::mutex> lock(m);
		c_enq.wait(lock, [this] { return data.size() != capacity; });
		data.push_back(x);
		c_deq.notify_one();
	}

	T dequeue() {
		std::unique_lock<std::mutex> lock(m);
		c_deq.wait(lock, [this] { return !data.empty(); });
		T ret = data.front();
		data.pop_front();
		c_enq.notify_one();
		return ret;
	}

private:
	std::mutex m;
	std::deque<T> data;
	size_t capacity;
	std::condition_variable c_enq;
	std::condition_variable c_deq;
};



#endif
