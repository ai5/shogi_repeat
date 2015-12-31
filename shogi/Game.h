#pragma once

#ifndef SHOGI_GAME_H_
#define SHOGI_GAME_H_

#include <memory>
#include <mutex>
#include <thread>


#include "DateTime.h"
#include "EnginePlayer.h"
#include "GameTimer.h"
#include "Handicap.h"
#include "MessageQueue.h"

#include "Notation.h"
#include "History.h"

struct PlayerSetting
{
	std::string Name;
	std::wstring WName;
	std::string FileName;
	std::string SettingFileName;

	// 時間設定
	int time  = 0;    // 持ち時間(秒
	int byoyomi = 1; // 秒読み(秒
	
};

struct GameParam
{
	PlayerSetting Black; // 先手設定
	PlayerSetting White; // 後手設定

	bool PonderON = false; // Ponder設定

	bool SaveLog = true;      // ログ保存
	bool SaveNotation = true; // 棋譜保存
	bool DispLog = false;      // ログをstdoutへ出力

	std::string LogPath = ""; // ログを保存するパス
	std::string KifPath = ""; // 棋譜を保存するパス

	Handicap handicap = Handicap::HIRATE; // 手合割

	// 棋譜の途中局面から開始
	bool UseNotation = false; // 
	Notation notation;  // 棋譜
	int Num = -1;            // 開始手数

	bool SwapPlayer = true; // 先後入れ替え

	int MaxPlays = 1; // 対局数
	int MaxMoves = 256; // 引き分けにする手数   <=0 で判定なし
};

class GameEngineListner : public EnginePlayerListener
{
	MessageQueue* msgq_;

public:
	GameEngineListner(MessageQueue* msgq)
	{
		this->msgq_ = msgq;
	}

	virtual ~GameEngineListner() {}

	virtual void notifyInisialized(Color color);
	virtual void notifyReadyOk(Color color);
	virtual void notifyBestMove(Color color, int transactionNo, const Move& bestmove, const Move& ponder);

	virtual void notifyInfo(Color color, int transactonNO, const PvInfo& str);

	virtual void notifyError(Color color, int error, const std::string& errorMsg);

};

// 連続対局するクラス
class Game
{
	std::thread th_; // 対局を動かすスレッド
	bool cancel_ = false; // キャンセルフラグ　キャンセルされると立つ
	bool terminate_ = false; // 終了フラグ
	bool play_ = false; // 対局中フラグ game_play からgame_endまで
	GameParam param_;

	std::unique_ptr<EnginePlayer> black_player_;
	std::unique_ptr<EnginePlayer> white_player_;

	GameEngineListner engine_listner_;
	
	MessageQueue msgq_; // メッセージQ

	std::mutex mtx_;

	Notation notation_; // 棋譜
	History history_; // 千日手判定用

	GameTimer timer_; // 時間切れ大麻

	DateTime start_date_; // 対局開始日時
	int count_ = 0;  // 対局数
	int black_win_ = 0;
	int white_win_ = 0;
	bool swap_flag = false;
public:
	Game();
	~Game();

	void Start(const GameParam& param);

	void Cancel() { this->cancel_ = true; this->msgq_.Send(std::make_shared<Message>(MessageID::Cancel)); }
	bool IsCancel() const { return this->cancel_; }

	void Terminate();
	void SaveOption(Color color, const std::string& filename);

private:

	void main_loop();
	bool is_continuous_game_end();

	void game_init();
	void game_start();
	void game_end();
	void game_term();

	void print_log();

	// イベント
	void EventPost(MessagePost& msg);
	void EventCancel(Message& msg);

	void EventTimeout(Message& msg);

	void EventInitialized(MessageInitialized& msg);
	void EventReady(MessageReady& msg);
	void EventBestMove(MessageBestMove& msg);


	// その他
	void LoadOption(Color color, const std::string& filename);
	std::string GetLogName(DateTime date);
	std::string GetKifuFileName(DateTime date, const std::string& black_name, const std::string& white_name);

	std::unique_ptr<EnginePlayer>& current_player() {
		return (this->notation_.position().side_to_move() == BLACK) ? this->black_player_ : this->white_player_;
	}

	std::unique_ptr<EnginePlayer>& opp_player() {
		return (this->notation_.position().side_to_move() == BLACK) ? this->white_player_ : this->black_player_;
	}

	void AddComment(const std::map<int, PvInfo>& pvinfos);

	// コールバック
	void timeout();

};

#endif
