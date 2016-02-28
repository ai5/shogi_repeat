
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
	: Num(num), TimeMs(timeMs), Score(score), Mate(mate), Nodes(nodes), Depth(depth), SelDepth(seldepth)
	, PvMoves(std::make_unique<Moves>(moves))
{
}

/*-----------------------------------------------------------------------------*/
/**
 * @brief 文字列変換
 * @note  
 */
/*-----------------------------------------------------------------------------*/
std::wstring PvInfo::ToString() const
{
	std::wstringstream ss;

	// multi pv
	if (this->Num != 0)
	{
		ss << L"候補" << this->Num;
	}

	// 時間
	{
		int min = this->TimeMs / (60 * 1000);
		int sec = (this->TimeMs - (min * 60 * 1000)) / 1000;
		int msec = (this->TimeMs - (min * 60 * 1000)) % 1000;

		ss << StringUtil::Format(L" 時間 %02d:%02d.%d", min, sec, msec / 100);
	}

	// 深さ
	{
		ss << L" 深さ " << this->Depth << L"/" << this->SelDepth;
	}

	// ノード数
	{
		ss << L" ノード数 " << this->Nodes;
	}

	if (this->Mate == MateNone)
	{
		int eval = this->Score;

		ss << L" 評価値 " << this->Score;

	}
	else
	{
		ss << L" 評価値 " << ((this->Mate < 0) ? L"-詰" : L"+詰");
		if (this->Score != 0)
		{
			ss << L" " << this->Score;
		}
	}

	// pv;
	if (this->PvMoves->size() != 0)
	{
		ss << L" 読み筋";
		for (auto& move : *this->PvMoves)
		{
			ss << L" " << ((move.side() == BLACK) ? L"▲" : L"△") <<  Kif::ToMoveString(move);
		}
	}

	return ss.str();
}