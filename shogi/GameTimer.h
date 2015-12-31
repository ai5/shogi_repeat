#pragma once

#ifndef SHOGI_GAME_TIMER_H_
#define SHOGI_GAME_TIMER_H_

#include <chrono>
#include <stdint.h>

#include "types.h"
#include "Timer.h"

struct GameRemainTime
{
	int Time;  // Ž‚¿ŽžŠÔŽc‚è (ms
	int Byoyomi; // •b“Ç‚ÝŽc‚èŽžŠÔ(ms
	int ElapsedTime;  // Á”ïŽžŠÔ(ms

	int HaveTime;     // Ž‚¿ŽžŠÔ(ms
	int HaveByoyomi;  // Ž‚¿•b“Ç‚ÝŽžŠÔ(ms
};

struct GameTime
{
	int Time       = 10 * 60 * 1000;  // Ž‚¿ŽžŠÔ (ms
	int RemainTime = 10 * 60 * 1000; // Žc‚èŽžŠÔ ( ms
	int Byoyomi    = 30 * 1000; // Ž‚¿ŽžŠÔ (ms
	int ElapsedTime = 0;  // Á”ïŽžŠÔ(ms

	int Remain() const { return this->RemainTime + this->Byoyomi; }
};

class Game;

class GameTimer
{
	Color turn_;
	bool started_;
	TimePoint start_time_;

	const int MarginTime = 300; // 300ms;
	Timer<Game*> timer_;
	Game* param;

public:
	GameTimer();
	GameTimer(std::function<void(Game*)> timeout, Game* param);

	~GameTimer();
	
	GameRemainTime GetRemainTime(Color color) const { return this->GetRemainTime(color, (color == BLACK) ? this->BlackTime : this->WhiteTime); }

	void SetTime(Color color, int time_min, int byoyomi_sec);
	void SetRestartTime(int blackTimeSec, int whiteTimeSec);

	void Start(Color turn);
	int Stop();

	int ChangeTurn();
	void TakeTurn();


protected:
	GameRemainTime GetRemainTime(Color turn, const GameTime& gameTime) const;

	int CalcTime(TimePoint now_time);
	
	TimePoint GetNowTime() const;
public:
	GameTime BlackTime;
	GameTime WhiteTime;

};


#endif
