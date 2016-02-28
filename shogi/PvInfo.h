#pragma once
#ifndef SHOGI_PV_INFO_H_
#define SHOGI_PV_INFO_H_

#include <string>
#include <memory>
class Moves;

struct PvInfo
{
	int Num;    // multipvの値　未設定の場合は0
	int TimeMs; // 時間
	int Score;    // スコア
	int Mate;	  // 0 通常スコア < 0 後手つみ > 0 先手つみ
	long long Nodes;  // ノード数
	int Depth;        // 深さ
	int SelDepth;
	std::unique_ptr<Moves> PvMoves; // 読み筋
	PvInfo() {};
	PvInfo(int num, int timeMs, int score, int mate, long long nodes, int depth, int seldepth, const Moves& moves);

	static const int MateNone = 0;
	static const int MateBlack = 1; // 先手から見て自分が勝ち
	static const int MateWhite = -1;  // 先手から見て相手が勝ち

	std::wstring ToString() const;
};




#endif
