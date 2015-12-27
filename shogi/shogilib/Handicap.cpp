
#include "Handicap.h"


const std::string HandicapExtention::HandicapString[HANDICAP_NB] =
{
	{ "平手"}, 
	{ "香落ち" }, 
	{ "右香落ち" }, 
	{ "角落ち" },   
	{ "飛車落ち" }, 
	{ "飛香落ち" }, 
	{ "二枚落ち" }, 
	{ "三枚落ち" }, 
	{ "四枚落ち" }, 
	{ "五枚落ち" }, 
	{ "左五枚落ち" },
	{ "六枚落ち" }, 
	{ "八枚落ち" }, 
	{ "十枚落ち" }, 
	{ "その他" },
};

/*-----------------------------------------------------------------------------*/
/**
 * @brief ハンディキャップ文字列を取得
 * @param handicap ハンディキャップ
 * @note  
 */
/*-----------------------------------------------------------------------------*/
const std::string& HandicapExtention::ToString(Handicap handicap)
{
	return HandicapString[handicap];
}

/*-----------------------------------------------------------------------------*/
/**
* @brief 手合割文字列からハンディキャップを取得
* @param str 文字列
* @note
*/
/*-----------------------------------------------------------------------------*/
Handicap HandicapExtention::FromString(const std::string& str)
{
	for (int i = 0; i < HANDICAP_NB; i++)
	{
		if (HandicapString[i] == str)
		{
			return Handicap(i);
		}
	}

	// ない場合
	return Handicap::OTHER;
}


