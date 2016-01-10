#pragma once

#ifndef SHOGI_TIMER_H_
#define SHOGI_TIMER_H_

#include <chrono>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <memory>

using TimePoint = std::chrono::milliseconds::rep;

template <class typeName>
class Timer
{
	std::thread th_;
	std::function<void(typeName)> timeout_func_ = nullptr; // コールバックのパラメータを変更する上手い仕組みはないのか？
	typeName timeout_param;

	bool close_ = false;
	bool start_ = false;
	int interval_ = 0;

	std::mutex mtx_;
	std::condition_variable cond_;
	TimePoint start_time_;


public:
	Timer() {
		this->th_ = std::thread(&Timer::timer_thread, this);
	}

	Timer(const Timer&) = delete;  // copyはできない
	Timer& operator=(const Timer&) = delete; // copyは出来ない

	~Timer() {
		this->Close();
	}
	
	void SetTimeOut(std::function<void(typeName)> func, typeName param) {
		this->timeout_func_ = func; 
		this->timeout_param = param;
	}

	// 開始 ms コールバックが呼び出されるまでの時間
	void Start(int ms) 
	{
		std::unique_lock<std::mutex> lock(this->mtx_);

		this->interval_ = ms;
		this->start_ = true;
		this->start_time_ = this->Now();
		this->cond_.notify_one();
	}

	// 停止
	void Stop() {
		std::unique_lock<std::mutex> lock(this->mtx_);
		this->start_ = false;
		this->cond_.notify_one();
	}

	// 閉じる
	void Close() {
		{
			std::unique_lock<std::mutex> lock(this->mtx_);
			this->close_ = true;
			this->start_ = false;
			this->cond_.notify_one();
		}

		if (th_.joinable())
		{
			th_.join();
		}
	}

	static TimePoint Now() {
		return std::chrono::duration_cast<std::chrono::milliseconds>
			(std::chrono::steady_clock::now().time_since_epoch()).count();
	}

protected:
	void timer_thread() 
	{
		std::unique_lock<std::mutex> lock(this->mtx_);

		while (!this->close_)
		{
			if (this->start_)
			{
				// 何故か抜けてくることがあるのでwhileループ
				while (this->start_)
				{
					TimePoint waittime = this->interval_ - (this->Now() - this->start_time_);
					if (waittime > 0)
					{
						std::cv_status sts = this->cond_.wait_for(lock, std::chrono::milliseconds(waittime));
					}

					TimePoint elapsed_time = (this->Now() - this->start_time_); // 経過時間ms

					if (elapsed_time >= this->interval_)
					{
						this->start_ = false; // one shot動作
											  // タイムアウト
						if (this->timeout_func_ != nullptr)
						{
							lock.unlock();
							this->timeout_func_(this->timeout_param);
							lock.lock();
						}

						break;
					}
				}
			}
			else
			{
				this->cond_.wait(lock);
			}
		}
	}
	
};

#endif


