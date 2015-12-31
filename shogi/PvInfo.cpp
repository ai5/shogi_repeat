
#include "PvInfo.h"

#include <sstream>

#include "Move.h"
#include "StringUtil.h"

#include "Kif.h"

/*-----------------------------------------------------------------------------*/
/**
 * @brief ÉRÉìÉXÉgÉâÉNÉ^
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
 * @brief ï∂éöóÒïœä∑
 * @note  
 */
/*-----------------------------------------------------------------------------*/
std::string PvInfo::ToString() const
{
	std::stringstream ss;

	// multi pv
	if (this->Num != 0)
	{
		ss << "åÛï‚" << this->Num;
	}

	// éûä‘
	{
		int min = this->TimeMs / (60 * 1000);
		int sec = (this->TimeMs - (min * 60 * 1000)) / 1000;
		int msec = (this->TimeMs - (min * 60 * 1000)) % 1000;

		ss << StringUtil::Format(" éûä‘ %02d:%02d.%d", min, sec, msec / 100);
	}

	// ê[Ç≥
	{
		ss << " ê[Ç≥ " << this->Depth << "/" << this->SelDepth;
	}

	// ÉmÅ[Éhêî
	{
		ss << " ÉmÅ[Éhêî " << this->Nodes;
	}

	if (this->Mate == MateNone)
	{
		int eval = this->Score;

		ss << " ï]âøíl " << this->Score;

	}
	else
	{
		ss << " ï]âøíl " << ((this->Mate < 0) ? "-ãl" : "+ãl");
		if (this->Score != 0)
		{
			ss << " " << this->Score;
		}
	}

	// pv;
	if (this->PvMoves->size() != 0)
	{
		ss << " ì«Ç›ãÿ";
		for (auto& move : *this->PvMoves)
		{
			ss << " " << ((move.side() == BLACK) ? "Å£" : "Å¢") <<  Kif::ToMoveString(move);
		}
	}

	return ss.str();
}