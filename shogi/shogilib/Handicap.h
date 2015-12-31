#pragma once

#ifndef SHOGI_HANDICAP_H_
#define SHOGI_HANDICAP_H_

#include <string>


enum Handicap
{
	HIRATE,    // 平手
	KYO,       // 香落ち
	RIGHT_KYO, // 右香車落ち
	KAKU,      // 角落ち
	HISYA,     // 飛車落ち
	HIKYO,     // 飛車香落ち
	H2,     // 2枚落ち
	H3,     // 3枚落ち
	H4,     // ４枚落ち
	H5,
	LEFT5, // 左５枚落ち
	H6,
	H8,
	H10,
	OTHER,
	HANDICAP_NB
};


class HandicapExtention
{

public:
	static const std::string HandicapString[HANDICAP_NB];
	static const std::wstring HandicapWString[HANDICAP_NB];

	/// 先手判定 true なら先手
	static bool IsBlack(Handicap handicap)
	{
		bool ret = false;

		if (handicap == Handicap::HIRATE || handicap == Handicap::OTHER)
		{
			ret = true;
		}

		return ret;
	}

	static const std::string& ToString(Handicap handicap);
	static Handicap FromString(const std::string& str);
	static Handicap FromWString(const std::wstring& str);

};


#endif
