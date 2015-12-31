#include <gtest/gtest.h>

#include <iostream>
#include <condition_variable>

#include "EnginePlayer.h"
#include "Semaphore.h"
#include "Timer.h"
#include "Move.h"
#include "Notation.h"
#include "GameTimer.h"
#include "PvInfo.h"


class Listener : public EnginePlayerListener 
{
	
	BinarySemaphore sem;
	int error_cnt_ = 0;

public:
	Listener() {}
	virtual ~Listener() {}

	virtual void notifyInisialized(Color )
	{
		sem.post();
	}

	virtual void notifyReadyOk(Color)
	{
		sem.post();
	}

	virtual void notifyBestMove(Color, int transactionNo, const Move& bestmove, const Move& ponder)
	{
		this->TransactionNo = transactionNo;
		sem.post();
	}

	virtual void notifyInfo(Color, int transactonNO, const PvInfo& info)
	{

	}

	virtual void notifyError(Color, int no, const std::string& error)
	{
		error_cnt_++;
		sem.post();
	}

	bool Wait(int ms)
	{
		if (ms < 0)
		{
			sem.wait();
			return true;
		}

		return sem.wait(ms);
	}

	int TransactionNo;
	int error_cnt() const { return this->error_cnt_; }
};

TEST(EnginePlayerTest, test)
{
	Listener listener;

	EnginePlayer player(BLACK, &listener);
	player.Init("tchild.exe");

	listener.Wait(10000); // 10秒
	// オプション送信

	player.Ready();
	listener.Wait(10000); // 10秒
	player.GameStart();

	Notation notation;
	GameTimer timer;

	int transactionNo = player.Go(notation, timer);
	ASSERT_EQ(transactionNo, 1);

	listener.Wait(1000);

	ASSERT_EQ(transactionNo, listener.TransactionNo);

	transactionNo = player.Go(notation, timer);
	ASSERT_EQ(transactionNo, 2);
	listener.Wait(1000);
	ASSERT_EQ(transactionNo, listener.TransactionNo);
	
	player.Terminate();
}

TEST(EnginePlayerTest, test2)
{
#if 0
	std::string str = EnginePlayer::time_str(Timer::Now());
	std::cout << str << std::endl;

	std::string str2 = EnginePlayer::time_str(Timer::Now());
	std::cout << str2 << std::endl;
#endif

}
TEST(EnginePlayerTest, test3)
{
#if 0
	Listener listener;

	EnginePlayer player(BLACK, &listener);
	player.Init("ErrorEngine.exe");

	listener.Wait(40000); // 10秒
						  // オプション送信
	ASSERT_EQ(listener.error_cnt(), 1);
#endif
}