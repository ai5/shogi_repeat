#pragma once

#ifndef SHOGI_SEMAPHORE_H_
#define SHOGI_SEMAPHORE_H_

#include <mutex>
#include <condition_variable>
#include <chrono>

class Semaphore
{
protected:
	std::mutex mtx_;
	std::condition_variable cond_;

	int count_;
	int max_;

public:
	Semaphore()
	{
		this->count_ = 0;
		this->max_ = INT_MAX;
	}

	Semaphore(int initial)
	{
		this->count_ = initial;
		this->max_ = INT_MAX;
	}

	Semaphore(int initial, int max)
	{
		this->count_ = initial;
		this->max_ = INT_MAX;
	}

	virtual ~Semaphore()
	{

	}


	void post()
	{
		std::unique_lock<std::mutex> lock(this->mtx_); // lock_guardを使うとこのスコープ内がロックされる

		if (this->count_ < this->max_)
		{
			this->count_++;
			cond_.notify_one();
		}
	}

	void wait()
	{
		std::unique_lock<std::mutex> lock(this->mtx_);

		while (this->count_ == 0)
		{
			this->cond_.wait(lock);
		}

		this->count_--;
	}

	bool wait(int timeoutMs)
	{
		std::unique_lock<std::mutex> lock(this->mtx_);

		while (this->count_ == 0)
		{
			// wait_for はunique_lockをアンロック
			// cond_が通知されるまで待つ
			// 指定時間で通知が来なければ timeout
			// 通知を受けたらunique_lockをロックする ・・・らしい

			if (timeoutMs < 0)
			{
				this->cond_.wait(lock);
			}
			else
			{
				if (this->cond_.wait_for(lock, std::chrono::milliseconds(timeoutMs)) == std::cv_status::timeout)
				{
					return false;
				}
			}
		}

		this->count_--;

		return true;
	}

};

class BinarySemaphore : public Semaphore
{
public:
	BinarySemaphore() : Semaphore(0, 1)	{}
	virtual ~BinarySemaphore() {}

	void Clear()
	{
		std::unique_lock<std::mutex> lock(this->mtx_);
		this->count_ = 0;
	}
};


#endif

