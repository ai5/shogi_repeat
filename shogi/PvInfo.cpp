
#include "PvInfo.h"

#include <sstream>

#include "Move.h"
#include "StringUtil.h"

#include "Kif.h"

/*-----------------------------------------------------------------------------*/
/**
 * @brief コンストラクタ
 * @note  
 */
/*-----------------------------------------------------------------------------*/
PvInfo::PvInfo(int num, int timeMs, int score, int mate, long long nodes, int depth, int seldepth, const Moves& moves)
{
	this->Num = num;
	this->TimeMs = timeMs;
	this->Score = score;
	this->Mate = mate;
	this->Nodes = nodes;
	this->Depth = depth;
	this->SelDepth = seldepth;
	this->PvMoves = std::make_unique<Moves>(moves);
}

/*-----------------------------------------------------------------------------*/
/**
 * @brief 文字列変換
 * @note  
 */
/*-----------------------------------------------------------------------------*/
std::string PvInfo::ToString() const
{
	std::stringstream ss;

	// multi pv
	if (this->Num != 0)
	{
		ss << "候補" << this->Num;
	}

	// 時間
	{
		int min = this->TimeMs / (60 * 1000);
		int sec = (this->TimeMs - (min * 60 * 1000)) / 1000;
		int msec = (this->TimeMs - (min * 60 * 1000)) % 1000;

		ss << StringUtil::Format(" 時間 %02d:%02d.%d", min, sec, msec / 100);
	}

	// 深さ
	{
		ss << " 深さ " << this->Depth << "/" << this->SelDepth;
	}

	// ノード数
	{
		ss << " ノード数 " << this->Nodes;
	}

	if (this->Mate == MateNone)
	{
		int eval = this->Score;

		ss << " 評価値 " << this->Score;

	}
	else
	{
		ss << " 評価値 " << ((this->Mate < 0) ? "-詰" : "+詰");
		if (this->Score != 0)
		{
			ss << " " << this->Score;
		}
	}

	// pv;
	if (this->PvMoves->size() != 0)
	{
		ss << " 読み筋";
		for (auto& move : *this->PvMoves)
		{
			ss << " " << ((move.side() == BLACK) ? "▲" : "△") <<  Kif::ToMoveString(move);
		}
	}

	return ss.str();
}