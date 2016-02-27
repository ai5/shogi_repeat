#pragma once
#ifndef ENGINE_PLAYER_H_
#define ENGINE_PLAYER_H_

#include <map>
#include <mutex>
#include <memory>
#include <thread>
#include <string>

#include "Move.h"
#include "Timer.h"
#include "Position.h"
#include "PvInfo.h"

class USIEngine;
class USIOptions;
class Notation;
class GameTimer;


// 内部状態
enum class EnginePlayerState {
	NONE,
	INITIALIZING, // usi -> usiok まで
	INISIALIZED,  // usiokからisreadyを送るまで
	WAIT_READY,   // isready -> readyokまで
	GO,           // go -> bestmove
	PONDER,       // go -> bestmove or go -> ponderhit -> bestmove
	STOP,         // stop -> bestmove
	IDLE,         // isreadyが終わったあと

	TERMINATING, // quit -> プロセス死亡まで
};


// イベント通知コールバック 
// ※color部分は識別子のほうがよいかも
class EnginePlayerListener
{
public:
	EnginePlayerListener() {}
	virtual ~EnginePlayerListener() {}

	virtual void notifyInisialized(Color color) = 0;
	virtual void notifyReadyOk(Color color) = 0;
	virtual void notifyBestMove(Color color, int transactionNo, const Move& bestmove, const Move& ponder) = 0;

	virtual void notifyInfo(Color color, int transactonNO, const PvInfo& info) = 0;

	virtual void notifyError(Color color, int errnoNo, const std::string& errorMsg) = 0;
};

// goリクエスト
struct GoRequest
{
	enum Type {
		NORMAL,   // 通常対局用
		TIME_INFINITY, // 時間無制限
		PONDER,   // 先読み
//		MATE,     // 詰み
//		MOVETIME,    // 時間他指定
	};

	GoRequest::Type ReqType;

	int Btime;
	int Wtime;
	int Byoyomi;

	// typeがMOVETIMEのとき以下の３つが使われる
	int64_t Nodes; // ノード数 マイナスの値だと未使用
	int Depth;     // 深さ
	int Time;      // 時間ms

	std::string Sfen = "";
	std::string Moves = "";
	int transactionNo;
	Position Pos;

	GoRequest() : ReqType(Type::TIME_INFINITY)
	{
	}

	GoRequest(int btime, int wtime, int byoyomi)
		: ReqType(Type::NORMAL)
		, Btime(btime)
		, Wtime(wtime)
		, Byoyomi(byoyomi)
	{
	}

	GoRequest(GoRequest::Type type)
		: ReqType(type)
	{
	}
};


class EnginePlayer
{
	Color color_;
	std::unique_ptr<USIEngine> engine_;
	std::unique_ptr<std::thread> th_;
	std::mutex mtx_;

	EnginePlayerState state_ = EnginePlayerState::NONE;

	bool cancel_ = false; // キャンセルフラグ
	EnginePlayerListener* lisnter_;
	
	int transactionCounter_ = 0;
	int transactionNo_;
	Position pos_;
	std::map<int, PvInfo> pv_info_;

	bool is_go_req_ = false;
	GoRequest go_req_;

	std::unique_ptr<Move> ponder_;

	std::string name_;
	std::string author_;

	Timer<EnginePlayer*> timer_;

public:
	EnginePlayer(Color color, EnginePlayerListener* lisnter);
	~EnginePlayer();


	bool Init(const std::string& filename);
	void Terminate();
	void Ready();

	int Go(Notation& notation, const GameTimer& time_info);
	int Ponder(const Notation& notation);
	void Stop();

	void Analyze();

	void GameStart();
	void GameOver(Color color);


	void SetOptions(const std::map<std::string, std::string>& opt_name_value);
	const USIOptions& GetOptions() const;

	const std::string& Name() const { return this->name_; }
	const std::string& Author() const { return this->author_; }

	const std::map<int, PvInfo>& PvInfos() const {
		return this->pv_info_;
	}

private:
	
	// コマンド受信処理
	void receive_thread();
	void receive_command(const std::string& str);
	void parse_option(const std::string& str);

	void parse_bestmove(const std::string& str);
	void parse_info(const std::string& str);
	void handleIdleState();

	void ExecGoReeust(const GoRequest& req);
	void send_cmd(const std::string &cmd);

	static std::string time_str(TimePoint tp);

	void InitTimeout();
};


#endif
