
#include <iostream>
#include <fstream>
#include <iomanip>
#include <map>
#include <sstream>
#include <string>

#include "types.h"
#include "Kif.h"
#include "Notation.h"
#include "Position.h"
#include "MoveCheck.h"
#include "StringUtil.h"



/*-----------------------------------------------------------------------------*/
static void Write(std::ostream& wr, const std::wstring& str);
static void Write(std::ostream& wr, const wchar_t* str);
static void WriteLine(std::ostream& wr, const std::wstring& str);
static void WriteLine(std::ostream& wr, const wchar_t* str);


const wchar_t* Kif::ArabiaNumber[] = { L"０", L"１", L"２", L"３", L"４", L"５", L"６", L"７", L"８", L"９" };
const wchar_t* Kif::KanNumber[] = { L"零", L"一", L"二", L"三", L"四", L"五", L"六", L"七", L"八", L"九", L"十" };
const wchar_t* Kif::KanjiPiece[] = { L"・", L"歩", L"香", L"桂", L"銀", L"角", L"飛", L"金", L"玉" };
const wchar_t* Kif::KanjiPiecePromotion[] = { L"・", L"と", L"杏", L"圭", L"全", L"馬", L"龍" };

const wchar_t* Kif::KanjiPiecePromotionStr[] = { L"・", L"と", L"成香", L"成桂", L"成銀", L"馬", L"龍" };

const wchar_t* Kif::TurnStrSengo[] = { L"先手", L"後手" };
const wchar_t* Kif::TurnStrSimokami[] = { L"下手", L"上手" };

const std::map<MoveType, const wchar_t*> Kif::ResultTable =
{
	{ MoveType::RESIGN ,     L"投了"},
	{ MoveType::STOP ,      L"中断" },
	{ MoveType::REPETITION, L"千日手" },
	{ MoveType::DRAW ,      L"持将棋" },
	{ MoveType::TIMEOUT ,   L"切れ負け"},
	{ MoveType::MATE ,      L"詰み"},
	{ MoveType::NO_MATE ,   L"不詰"},
	{ MoveType::LOSE_FOUL,  L"反則負け"},
	{ MoveType::WIN_FOUL,   L"反則勝ち"},
	{ MoveType::STOP,       L"封じ手"  },
};

const std::map<std::wstring, MoveType> Kif::ResultFromWStringTable =
{
	{ L"投了",     MoveType::RESIGN },
	{ L"中断",     MoveType::STOP },
	{ L"千日手",   MoveType::REPETITION },
	{ L"持将棋",   MoveType::DRAW },
	{ L"切れ負け", MoveType::TIMEOUT },
	{ L"詰み",     MoveType::MATE },
	{ L"不詰",     MoveType::NO_MATE },
	{ L"反則負け", MoveType::LOSE_FOUL },
	{ L"反則勝ち", MoveType::WIN_FOUL },
	{ L"封じ手",   MoveType::STOP },
};

const std::map<std::wstring, Piece> Kif::WidePieceTable =
{
	{ L"・", NO_PIECE },
	{ L"歩", B_PAWN },
	{ L"香", B_LANCE },
	{ L"桂", B_KNIGHT },
	{ L"銀", B_SILVER },
	{ L"角", B_BISHOP },
	{ L"飛", B_ROOK },
	{ L"金", B_GOLD },
	{ L"玉", B_KING },
	{ L"王", B_KING },

	{ L"と", B_PRO_PAWN},

	{ L"成香", B_PRO_LANCE },
	{ L"杏", B_PRO_LANCE },
	
	{ L"成桂", B_PRO_KNIGHT },
	{ L"圭", B_PRO_KNIGHT },

	{ L"成銀", B_PRO_SILVER },
	{ L"全", B_PRO_SILVER },

	{ L"馬", B_PRO_BISHOP},
	{ L"龍", B_PRO_ROOK },
	{ L"竜", B_PRO_ROOK },
};

const std::wstring Kif::WideKanjiNumber = L"零一二三四五六七八九十";
const std::wstring Kif::WideArabicNumber = L"０１２３４５６７８９";


/*-----------------------------------------------------------------------------*/
/**
 */
/*-----------------------------------------------------------------------------*/
Kif::Kif()
{
}


Kif::~Kif()
{
}


/*-----------------------------------------------------------------------------*/
/**
 * @brief Kif判定
 * @param str kifの先頭一部分
 * @note  
 */
/*-----------------------------------------------------------------------------*/
bool Kif::CanRead(const std::string& str)
{
	return true;
}

/*-----------------------------------------------------------------------------*/
/**
@brief ファイルから読み込み
@param notation 棋譜クラス
@param filename ファイル名
@note sfen形式の棋譜を読み込む
*/
/*-----------------------------------------------------------------------------*/
bool Kif::Load(Notation& notation, const std::string& filename)
{
	std::ifstream file;

	notation.Init();

	file.open(filename.c_str());

	if (!file.is_open()) {
		std::cerr << "file open error" << filename << std::endl;
		return false;
	}

	if (!file.eof())
	{
		// 読み込み
		Kif::ReadNotation(notation, file);
	}

	file.close();

	return true;
}

/*-----------------------------------------------------------------------------*/
/**
@brief ファイルに保存
@param notation 棋譜クラス
@param filename ファイル名
@note sfen形式でファイルに保存する。
*/
/*-----------------------------------------------------------------------------*/
bool Kif::Save(Notation& notation, const std::string& filename)
{
	std::ofstream file;

	file.open(filename.c_str());

	if (!file.is_open())
	{
		std::cerr << "file open error" << filename << std::endl;
		return false;
	}

	// 保存
	this->WriteNotation(notation, file);

	file.close();

	return true;
}


/*-----------------------------------------------------------------------------*/
/**
* @brief 文字列から読み込み
* @param notation 棋譜クラス
* @param str　棋譜文字列
* @note
*/
/*-----------------------------------------------------------------------------*/
void Kif::LoadFromString(Notation& notation, const std::string& str)
{
	std::stringstream ss(str);

	notation.Init();

	// 読み込み
	Kif::ReadNotation(notation, ss);
	
}


/*-----------------------------------------------------------------------------*/
/**
@brief Kif文字列で返す
@param notation 棋譜クラス
@return Kif文字列
*/
/*-----------------------------------------------------------------------------*/
std::string Kif::SaveToString(const Notation& notation)
{
	std::stringstream ss;


	this->WriteNotation(notation, ss);

	return ss.str();
}

/*-----------------------------------------------------------------------------*/
/**
@brief 局面情報を文字列で返す
@param notation 棋譜クラス
@return BOD文字列
*/
/*-----------------------------------------------------------------------------*/
std::string Kif::ToBodString(const Notation& notation)
{
	std::stringstream ss;

	Kif::WritePosition(notation.position(), notation.handicap(), ss);

	return ss.str();
}

/*-----------------------------------------------------------------------------*/
/**
 * @brief 棋譜読み込み
 * @param notation 棋譜クラス
 * @param sr
 * @note  
 */
/*-----------------------------------------------------------------------------*/
void Kif::ReadNotation(Notation& notation, std::istream& sr)
{
	this->pos_rank_ = RANK_1;
	Position position;

	while (!sr.eof())
	{
		std::string line;

		std::getline(sr, line);
		
		std::wstring wstr = StringUtil::TrimStart(StringUtil::ConvertWStringFromString(line), L" 　");

		wstr = StringUtil::Trim(wstr, L"\r\n"); // \rのみでおｋ？

		if (line == "" || line[0] == '#')
		{
			// 空行もしくは先頭が#
			continue;
		}

		if (wstr[0] == L'*')
		{
			// コメントの場合
			notation.AddComment(wstr.substr(1));
		}
		else
		{
			// コメント行以外
			if (this->ReadHeader(notation, wstr))
			{
			}
			else if (this->ReadPosition(notation, position, wstr))
			{
			}
			else if (this->ReadMove(notation, wstr))
			{

			}
			else
			{
				// エラー
				break;
			}
		}
	}
}

/*-----------------------------------------------------------------------------*/
/**
* @brief ヘッダの読み込み
* @param notation 棋譜クラス
* @param line
* @note
*/
/*-----------------------------------------------------------------------------*/
bool Kif::ReadHeader(Notation& notation, const std::wstring& line)
{
	int pos = StringUtil::IndexOfAny(line, L":：");
	
	int ignore_pos = StringUtil::IndexOfAny(line, L"|(（");
	
	bool ret = false;

	if (ignore_pos != -1 && ignore_pos < pos)
	{
		return false;
	}

	if (pos != -1)
	{
		ret = true;

		// ほげ：ほげとみなす
		std::wstring key = line.substr(0, pos);
		std::wstring value = line.substr(pos + 1);
		int temp;

		if (key.find(' ') != std::wstring::npos
			&& (Kif::IsSankaku(key[0]) || StringUtil::ParseNum(key, OUT temp)) )
		{
			// KEY部分がスペースが含まれている && (先頭が三角 or 数値)
			// 指し手じゃなかろうか
			return false;
		}

		if (key == L"手合割")
		{
			value = StringUtil::Trim(value, L" 　");

			Handicap handicap = HandicapExtention::FromWString(value);

			notation.set_handicap(handicap);
		}
		else if (key == L"先手" || key == L"下手")
		{
			// 先手名前
			notation.set_black_name(value);
		}
		else if (key == L"後手" || key == L"上手")
		{
			notation.set_white_name(value);
		}
		else if (key == L"先手の持駒" || key == L"下手の持駒")
		{
			ret = false; // Positionで判別するのでここではスキップ
		}
		else if (key == L"後手の持駒" || key == L"上手の持駒")
		{
			ret = false; // Positionで判別するのでここではスキップ
		}
		else if (key == L"変化")
		{
			// 指し手で判別するので、ここではスキップ?
			ret = false;
		}
		else
		{
			if (value != L"")
			{
				notation.set_kifu_info(key, value);
			}

			// Debug.WriteLine("{0} / {1}", line, key);
		}
	}


	return ret;
}

/*-----------------------------------------------------------------------------*/
/**
* @brief Position情報読み込み
* @param notation 棋譜クラス
* @param line
* @note
*/
/*-----------------------------------------------------------------------------*/
bool Kif::ReadPosition(Notation&notation, Position& position,  const std::wstring& line)
{
	if (StringUtil::StartsWith(line, L"先手番") || StringUtil::StartsWith(line, L"下手番"))
	{
		position.set_side_to_move(BLACK);
		notation.SetInitialPosition(position);
	}
	else if (StringUtil::StartsWith(line, L"後手番") || StringUtil::StartsWith(line, L"上手番"))
	{
		position.set_side_to_move(WHITE);
		notation.SetInitialPosition(position);
	}
	else if (StringUtil::StartsWith(line, L"９ ８ ７ ６ ５ ４ ３ ２ １"))
	{
	}
	else if (StringUtil::StartsWith(line, L"+---------------------------+"))
	{
	}
	else if (StringUtil::StartsWith(line, L"手数----指手---"))
	{

	}
	else if (StringUtil::StartsWith(line, L"先手の持駒") || StringUtil::StartsWith(line, L"下手の持駒"))
	{
		this->ReadHand(BLACK, position, line, 6);
		notation.SetInitialPosition(position);
	}
	else if (StringUtil::StartsWith(line, L"後手の持駒") || StringUtil::StartsWith(line, L"上手の持駒"))
	{
		this->ReadHand(WHITE, position, line, 6);
		notation.SetInitialPosition(position);
	}
	else if (line[0] == '|')
	{
		// 盤面
		if (this->pos_rank_ < RANK_NB && line.length() >= 10)
		{
			size_t idx = 1;

			for (File file = FILE_9; file < File::FILE_NB && idx < (int)line.length(); ++file)
			{
				Color color = BLACK;
				Piece piece = Piece::NO_PIECE;
				
				if (line[idx] == 'v' || line[idx] == 'V' || line[idx] == '-')
				{
					color = WHITE;
					idx++;
				}
				else if (line[idx] == ' ' || line[idx] == '+' || line[idx] == '^')
				{
					idx++;
				}
				
				// 通常駒
				piece = Kif::PieceFromWStr(line, &idx);

				if (color == WHITE)
				{
					piece = opp(piece); // 後手
				}

				position.SetPiece(file, this->pos_rank_, piece);
			}

			this->pos_rank_ += 1;

			if (this->pos_rank_ >= RANK_NB)
			{
				notation.SetInitialPosition(position);
			}
		}
	}
	else 
	{
		return false;
	}

	return true;
}

/*-----------------------------------------------------------------------------*/
/**
* @brief 持ち駒の読み込み
* @param position 
* @param line
* @note
*/
/*-----------------------------------------------------------------------------*/
bool Kif::ReadHand(Color color, Position& pos, const std::wstring& str, int ofs)
{
	Piece piece;
	int num;

	for (size_t i = (size_t)ofs; i < str.length(); i++)
	{
		wchar_t ch = str[i];
		if (ch == ' ' || ch == L'　')
		{
			// スペース
			continue;
		}

		size_t temp = i;
		piece = PieceFromWStr(str, &temp);

		
		if (piece == Piece::NO_PIECE)
		{
			// 変換失敗
			continue;
		}

		num = 1;
		if ((i + 1) < str.length())
		{
			temp = i + 1;

			num = NumberFromKanjiNumber(str, &temp);

			i += (temp - (i + 1));

			if (num == 0)
			{
				num = 1;
			}
		}

		if (color == WHITE)
		{
			pos.SetWhiteHand(type_of(piece), num);
		}
		else
		{
			pos.SetBlackHand(type_of(piece), num);
		}
	}

	return true;

}

/*-----------------------------------------------------------------------------*/
/**
* @brief 指し手部分を読み込み
* @param notation 棋譜クラス
* @param line 読み込んだ行
* @note
*/
/*-----------------------------------------------------------------------------*/
bool Kif::ReadMove(Notation& notation, const std::wstring& line)
{
	bool ret = true;
	if (StringUtil::StartsWith(line, L"変化"))
	{
		int pos = StringUtil::IndexOfAny(line, L":：");

		std::wstring value = StringUtil::Trim(line.substr(pos + 1), L" 　");

		int num;

		if (!StringUtil::ParseNum(value, OUT num) || num == 0)
		{
			return false;
		}
			
		// カレント分岐の番号-の局面へ移動1
		notation.Jump(num - 1);
	}
	else
	{
		if (Kif::IsSankaku(line[0]))
		{
			// ki2の指し手読み込み
			return false;
		}
		else
		{
			ret = ReadKifMove(notation, line);
		}
	}

	return ret;
}

/*-----------------------------------------------------------------------------*/
/**
* @brief Kif指し手部分を読み込み
* @param notation 棋譜クラス
* @param line 読み込んだ行
* @note
*/
/*-----------------------------------------------------------------------------*/
bool Kif::ReadKifMove(Notation& notation, const std::wstring& line)
{
	std::deque<std::wstring> str_list;
	Kif::SplitMove(str_list, line);

	if (str_list.size() < 2)
	{
		// 行番号と指し手がない場合はreturn
		return false;
	}

	int number;
	if (!StringUtil::ParseNum(str_list[0], OUT number))
	{
		return false;
	}

	MoveKif move;

	if (!Kif::ParseKifMove(notation, str_list[1], OUT move))
	{
		return false;
	}

	if (str_list.size() >= 3)
	{
		move.time = StringUtil::ParseTime(str_list[2]);
	}

	// ターンのチェック
	if (move.side() != notation.position().side_to_move())
	{
		// sideが違う場合 たまに手番間違いで終了した棋譜でこのような場合がある
		notation.AddMove(MoveKif(MoveType::WIN_FOUL)); // 反則勝ち
		return false;
	}

	bool move_valid = true;

	if (is_move(move.move_type()))
	{
		move_valid = MoveCheck::IsValid(notation.position(), move); // 合法てチェック();
	}

	// 指し手追加
	bool ret = notation.AddMove(move); // 非合法手でもそのまま棋譜に追加

	// 非合法手ならエラー
	if (!move_valid)
	{
		ret = false;
		notation.AddMove(MoveKif(MoveType::WIN_FOUL)); // 反則勝ち
	}

	return ret;
}


/*-----------------------------------------------------------------------------*/
/**
 * @brief ストリームへ棋譜を出力
 * @param notation 棋譜
 * @param wr 出力ストリーム
 * @note  
 */
/*-----------------------------------------------------------------------------*/
void Kif::WriteNotation(const Notation& notation, std::ostream& wr)
{
	this->turn_str_ = Kif::TurnStrSengo;

	if (!HandicapExtention::IsBlack(notation.handicap()))
	{
		turn_str_ = TurnStrSimokami;
	}
	
	// 他の情報
	for (auto& de : notation.kifu_infos())
	{
		WriteLine(wr, de.first + L"：" + de.second);
	}

	if (notation.output_initial_position())
	{
		Kif::WritePosition(notation.initial_position(), notation.handicap(), wr);
	}
	else
	{
		// 手合割の出力
		WriteLine(wr, L"手合割：" + HandicapExtention::ToWString(notation.handicap()));
	}

	// 先手：名前　の出力
	WriteLine(wr, std::wstring(this->turn_str_[BLACK]) + L"：" + notation.black_name());
	WriteLine(wr, std::wstring(this->turn_str_[WHITE]) + L"：" + notation.white_name());

	WriteLine(wr, L"手数----指手---------消費時間--");

	// 指し手出力
 	this->WriteMoveTree(notation.moves(), wr);
}

/*-----------------------------------------------------------------------------*/
/**
 * @brief 局面情報出力
 * @param pos 局面情報
 * @param handicap ハンディキャップ (後手、上手の切り替えに使う
 * @note  
 */
/*-----------------------------------------------------------------------------*/
void Kif::WritePosition(const Position& pos, Handicap handicap, std::ostream& wr)
{
	std::wstring hand_str = Kif::HandStrFromHand(pos.hand()[WHITE]);

	if (HandicapExtention::IsBlack(handicap))
	{
		WriteLine(wr, L"後手の持駒：" + hand_str);
	}
	else
	{
		WriteLine(wr, L"上手の持駒：" + hand_str );
	}

	WriteLine(wr, L"  ９ ８ ７ ６ ５ ４ ３ ２ １");
	WriteLine(wr, L"+---------------------------+");

	for (Rank rank = RANK_1; rank < RANK_NB; ++rank)
	{
		Write(wr, L"|");

		for (File file = FILE_9; file < FILE_NB; ++file)
		{
			Piece piece = pos.GetPiece(file, rank);

			if (color_of(piece) == Color::WHITE)
			{
				Write(wr, L"v");
			}
			else
			{
				Write(wr, L" ");
			}

			// 駒書き出し
			Write(wr, Kif::PieceCharFromPiece(piece));
		}

		WriteLine(wr, std::wstring(L"|") + Kif::KanNumber[rank + 1]);
	}

	WriteLine(wr, L"+---------------------------+");

	hand_str = Kif::HandStrFromHand(pos.hand()[BLACK]);

	if (HandicapExtention::IsBlack(handicap))
	{
		WriteLine(wr, L"先手の持駒：" + hand_str);
	}
	else
	{
		WriteLine(wr, L"下手の持駒：{0}" + hand_str );
	}

	if (pos.side_to_move() == WHITE)
	{
		if (HandicapExtention::IsBlack(handicap))
		{
			WriteLine(wr, L"後手番");
		}
		else
		{
			WriteLine(wr, L"下手番");
		}
	}
}

/*-----------------------------------------------------------------------------*/
/**
* @brief 指し手を分岐も含めて出力する
* @param moves 指し手
* @param wr ライタ
* @note
*/
/*-----------------------------------------------------------------------------*/
void Kif::WriteMoveTree(const Moves& moves, std::ostream& wr)
{

	this->WriteMoves(moves, wr);

	for (Moves::const_reverse_iterator ite = moves.rbegin(); ite != moves.rend(); ++ite)
	{
		for (auto& mvs : ite->branches())
		{
			WriteLine(wr, StringUtil::Format(L"変化：%d手", ite->number()));
			Kif::WriteMoveTree(*mvs, wr);
		}
	}
}

/*-----------------------------------------------------------------------------*/
/**
* @brief 指し手の出力
* @param moves 指し手
* @param wr ライタ
* @note
*/
/*-----------------------------------------------------------------------------*/
void Kif::WriteMoves(const Moves& moves, std::ostream& wr)
{
	// std::ios::fmtflags flags = wr.flags();

	for (auto& move : moves)
	{
		if (move.number() != 0)
		{
			std::wstring str = Kif::ToMoveString(move);
			int width = StringUtil::WStringWidth(str);
			for (; width < 13; width++)
			{
				str += L' ';
			}
			
#ifdef _WIN32
			WriteLine(wr, StringUtil::Format(L"%4d %s (%s)", move.number(), str.c_str(), Kif::ToMoveTimeString(move).c_str()));
#else
			WriteLine(wr, StringUtil::Format(L"%4d %ls (%ls)", move.number(), str.c_str(), Kif::ToMoveTimeString(move).c_str()));
#endif
			// wr.WriteLine("{0,4} {1,-11}    ({2})", move_info.Number, GetMoveString(move_info), GetMoveTimeString(move_info));

			for (auto& comment : move.comments())
			{
				WriteLine(wr, L"*" + comment);
			}
		}
	}

	auto& move = moves.back();

	if (is_result(move.move_type()))
	{
		// 終端がResultの場合

		Color rev_turn = opp(move.side()); // 反対側

		switch (move.move_type())
		{
		case MoveType::RESIGN: // 投了
			// wr << "まで" << (move.number() - 1) << "手で" << this->turn_str_[rev_turn] << "の勝ち" << std::endl;
#ifdef _WIN32
			WriteLine(wr, StringUtil::Format(L"まで%d手で%sの勝ち", move.number() - 1, this->turn_str_[rev_turn]));
#else
			WriteLine(wr, StringUtil::Format(L"まで%d手で%lsの勝ち", move.number() - 1, this->turn_str_[rev_turn]));
#endif
			// wr.WriteLine("まで{0}手で{1}の勝ち", move_info.Number - 1, this.turnStr[(int)rev_turn]);
			break;
		case MoveType::TIMEOUT: // 時間切れ
			// wr << "まで" << move.number() << "手で時間切れにより" << this->turn_str_[rev_turn] << "の勝ち" << std::endl;
#ifdef _WIN32
			WriteLine(wr, StringUtil::Format(L"まで%d手で時間切れにより%sの勝ち", move.number() - 1, this->turn_str_[rev_turn]));
#else
			WriteLine(wr, StringUtil::Format(L"まで%d手で時間切れにより%lsの勝ち", move.number() - 1, this->turn_str_[rev_turn]));
#endif
			// wr.WriteLine("まで{0}手で時間切れにより{1}の勝ち", move_info.Number - 1, this->turn_str_[(int)rev_turn]);
			break;
		case MoveType::WIN_FOUL: // 反則勝ち
		case MoveType::LOSE_FOUL: // 反則負け
			// wr << "まで" << move.number() << "手で" << this->turn_str_[move.side()] << "の" << Kif::MoveTypeStrFromMoveType(move.move_type()) << std::endl;
#ifdef _WIN32
			WriteLine(wr, StringUtil::Format(L"まで%d手で%sの%s", move.number() - 1, this->turn_str_[move.side()], Kif::MoveTypeStrFromMoveType(move.move_type())));
#else
			WriteLine(wr, StringUtil::Format(L"まで%d手で%lsの%ls", move.number() - 1, this->turn_str_[move.side()], Kif::MoveTypeStrFromMoveType(move.move_type())));
#endif
			// wr.WriteLine("まで{0}手で{1}の{2}", move_info.Number - 1, this.turnStr[(int)move_info.Turn], StrFromMovetype(move_info.MoveType));
			break;

		default:
			// wr << "まで" << move.number() << "手で" << Kif::MoveTypeStrFromMoveType(move.move_type()) << std::endl;
#ifdef _WIN32
			WriteLine(wr, StringUtil::Format(L"まで%d手で%sの%s", move.number() - 1, Kif::MoveTypeStrFromMoveType(move.move_type())));
#else
			WriteLine(wr, StringUtil::Format(L"まで%d手で%lsの%ls", move.number() - 1, Kif::MoveTypeStrFromMoveType(move.move_type())));
#endif
			// wr.WriteLine("まで{0}手で{1}", move_info.Number - 1, StrFromMovetype(move_info.MoveType));
			break;
		}

	}
}

/*-----------------------------------------------------------------------------*/
/**
 * @brief 駒の文字を返す
 * @param piece 駒
 * @return 駒文字
 * @note  
 */
/*-----------------------------------------------------------------------------*/
const wchar_t* Kif::PieceCharFromPiece(Piece piece)
{
	PieceType pt = type_of(piece);


	if (is_promoted(piece))
	{
		return Kif::KanjiPiecePromotion[pt];
	}

	return Kif::KanjiPiece[pt];
}

/*-----------------------------------------------------------------------------*/
/**
* @brief 駒の文字を返す
* @param piece 駒
* @return 駒文字
* @note
*/
/*-----------------------------------------------------------------------------*/
const wchar_t* Kif::PieceStrFromPiece(Piece piece)
{
	PieceType pt = type_of(piece);


	if (is_promoted(piece))
	{
		return Kif::KanjiPiecePromotionStr[pt];
	}

	return Kif::KanjiPiece[pt];
}


/*-----------------------------------------------------------------------------*/
/**
* @brief 駒の文字を返す
* @param piece 駒
* @return 駒文字
* @note
*/
/*-----------------------------------------------------------------------------*/
std::wstring Kif::HandStrFromHand(const int hand[PIECE_TYPE_NB])
{
	int num;
	static const PieceType hand_table[] = {
		ROOK, BISHOP, GOLD, SILVER, KNIGHT, LANCE, PAWN,
	};

	int total = 0;
	for (auto pt : hand_table)
	{
		total += hand[pt];
	}

	if (total == 0)
	{
		return L"なし";
	}

	std::wstring str = L"";

	for (auto pt : hand_table)
	{
		num = hand[pt];

		if (num != 0)
		{
			str += Kif::KanjiPiece[pt];
			if (num > 1)
			{
				// 複数持っている
				if (num > 10)
				{
					str += KanNumber[10];
					num = num % 10;
				}

				if (num > 0)
				{
					str += KanNumber[num];
				}
			}

			str += ' ';
		}
	}

	return str;
}
/*-----------------------------------------------------------------------------*/
/**
* @brief 駒の文字を返す
* @param piece 駒
* @return 駒文字
* @note
*/
/*-----------------------------------------------------------------------------*/
const wchar_t* Kif::MoveTypeStrFromMoveType(MoveType movetype)
{
	if (Kif::ResultTable.count(movetype) > 0)
	{
		return Kif::ResultTable.at(movetype);
	}

	return L"";
}


/*-----------------------------------------------------------------------------*/
/**
 * @brief kiu用指し手文字列の取得
 * @param 
 * @note  
 */
/*-----------------------------------------------------------------------------*/
std::wstring Kif::ToMoveString(const Move& move)
{
	std::wstring str = L"";

	int dan, suji;

	if (move.move_type() == MoveType::PASS)
	{
		str = L"パス";
	}
	else if (is_move(move.move_type()))
	{
		// 通常の指し手の場合 移動先を設定
		if (move.move_type() & MoveType::SAME_FLAG)
		{
			str = L"同　";
		}
		else
		{
			dan = dan_from_rank(rank_of(move.to()));
			suji = suji_from_file(file_of(move.to()));

			str = std::wstring(Kif::ArabiaNumber[suji]) + Kif::KanNumber[dan];
		}

		// 駒文字列を入れる
		str += Kif::PieceStrFromPiece(move.piece());

		if (move.move_type() & MoveType::PROMOTION_FLAG)
		{
			str += L"成";
		}

		if (move.move_type() & MoveType::DROP_FLAG)
		{
			str += L"打";
		}
		else 
		{
			// 普通の移動か成る場合移動元を入れる
			dan = dan_from_rank(rank_of(move.from()));
			suji = suji_from_file(file_of(move.from()));

			str += StringUtil::Format(L"(%d%d)", suji, dan);
		}
	}
	else if (move.move_type() & MoveType::RESULT_FLAG)
	{
		str = MoveTypeStrFromMoveType(move.move_type());
	}
	else
	{
		str = L"";
	}

	return str;
}

/*-----------------------------------------------------------------------------*/
/**
 * @brief 指し手時間文字列取得
 * @param 
 * @note  
 */
/*-----------------------------------------------------------------------------*/

std::wstring Kif::ToMoveTimeString(const MoveKif& info)
{
	std::string str;
	int hour;
	int time;

	hour = info.totalTime / (60 * 60);
	time = info.totalTime - (hour * (60 * 60));


	return StringUtil::Format(
		L"%2d:%02d/%02d:%02d:%02d", 
		info.time / 60,
		info.time % 60,
		hour,
		time / 60,
		time % 60
		);
#if 0
	str = string.Format(
		"{0,2}:{1:00}/{2:00}:{3:00}:{4:00}",
		info.Time / 60,
		info.Time % 60,
		hour,
		time / 60,
		time % 60);
		return str;
#endif
}

/*-----------------------------------------------------------------------------*/
/**
 * @brief 文字列からコマ番号を返す
 * @param str 文字列
 * @param ref_pos[in,out] 文字の位置
 * @note  
 */
/*-----------------------------------------------------------------------------*/
Piece Kif::PieceFromWStr(const std::wstring& str, size_t* ref_pos)
{
	Piece piece = Piece::NO_PIECE;
	size_t pos = 0;

	if (ref_pos == nullptr)
	{
		ref_pos = &pos;
	}

	if (*ref_pos >= str.length())
	{
		return piece;
	}

	std::wstring piece_str;

	if (str[*ref_pos] == L'成')
	{
		if ((*ref_pos + 1) >= (int)str.length())
		{
			return piece;
		}

		piece_str = str.substr(*ref_pos, 2);
		*ref_pos += 2;
	}
	else
	{
		piece_str = str.substr(*ref_pos, 1);
		*ref_pos += 1;
	}

	if (Kif::WidePieceTable.count(piece_str) > 0)
	{
		piece = Kif::WidePieceTable.at(piece_str);
	}

	return piece;
}

/*-----------------------------------------------------------------------------*/
/**
 * @brief 漢数字２桁までを数値に直す
 * @note  ～十九までパースできればOk
 */
/*-----------------------------------------------------------------------------*/
int Kif::NumberFromKanjiNumber(std::wstring str, size_t* index)
{
	int num = 0;
	size_t idx;

	for (idx = *index ; idx < (int)str.length(); idx++)
	{
		wchar_t ch = str[idx];

		if (ch == ' ' || ch == L'　')
		{
			// スペース
		}
		else
		{
			size_t pos = Kif::WideKanjiNumber.find(ch);

			if (pos == std::wstring::npos)
			{
				// 数値じゃない？
				break;
			}
			else
			{
				num = num + pos;
			}
		}
	}

	*index = idx;

	return num;
}

/*-----------------------------------------------------------------------------*/
/**
* @brief 漢字を含んだ数値文字列を数値に直す
* @param ch 数値文字
* @return 数値 無ければマイナスを返す
*/
/*-----------------------------------------------------------------------------*/
int Kif::NumberFromWChar(wchar_t ch)
{
	if (ch >= '0' && ch <= '9')
	{
		return ch - '0';
	}

	size_t pos = Kif::WideKanjiNumber.find(ch);
	if (pos == std::wstring::npos)
	{
		pos = Kif::WideArabicNumber.find(ch);
		if (pos == std::wstring::npos)
		{
			return -1;
		}
	}

	return (int)pos;
}

// 指し手の区切り文字判定
bool Kif::IsMoveSeparator(wchar_t ch)
{
	if (ch == ' ' || ch == '(' || ch == ')' || ch == '/' || ch == '\t')
	{
		return true;
	}

	return false;
}

bool Kif::IsMoveSeparatorEnd(wchar_t ch)
{
	if (ch == ' ' || ch == '/' || ch == '\t')
	{
		return true;
	}

	return false;
}

/// <summary>
/// 三角の判定
/// </summary>
/// <param name="ch"></param>
/// <returns></returns>
bool Kif::IsSankaku(wchar_t ch)
{
	bool ret = false;

	if (ch == L'▲' || ch == L'△' || ch == L'▽' || ch == L'▼')
	{
		ret = true;
	}

	return ret;
}

/*-----------------------------------------------------------------------------*/
/**
 * @brief 指し手文字列を分割
 * @param 
 * @note  " 1 ２四歩(23)   ( 0:05/00:00:05)" を
 *        "1" "２四歩(23)" "0:05" "00:00:05"
 *      "△３四歩    ▲２六歩"を
 *      "△３四歩" "▲２六歩"
 */
/*-----------------------------------------------------------------------------*/
void Kif::SplitMove(std::deque<std::wstring>& str_list, const std::wstring& line)
{
	str_list.clear();

	// 先頭の空白削除
	size_t start;
	size_t end;

	start = 0;

	while (start < line.length())
	{
		// 区切り文字のスキップ
		for (; start < line.length(); start++)
		{
			if (!Kif::IsMoveSeparator(line[start]))
			{
				// 区切り文字以外なら
				break;
			}
		}

		// 文字列切り出し
		for (end = start; end < line.length(); end++)
		{
			// 区切りもじなら
			if (line[end] == ' ' && end != 0 && line[end - 1] == L'同')
			{
				// 一つ前が同？
			}
			else if (line[end] == ')')
			{
				end++;
				break;
			}
			else if (Kif::IsMoveSeparatorEnd(line[end]))
			{
				break;
			}

			if (end != start)
			{
				if (Kif::IsSankaku(line[end]))
				{
					// 単語の先頭以外の三角は終了させる
					break;
				}
			}
		}

		if (end == start)
		{
			// 文字列が０個
			break;
		}

		str_list.emplace_back(line.substr(start, end - start));

		start = end;
	}

}

/*-----------------------------------------------------------------------------*/
/**
* @brief 指し手文字列から指し手を返す
* @param str 指し手文字列
* @param [out] move 指し手
* @return true パース成功
          false 失敗
*/
/*-----------------------------------------------------------------------------*/
bool Kif::ParseKifMove(const Notation& notation, const std::wstring& str, MoveKif& move)
{
	Color color = notation.position().side_to_move();

	size_t ofs = 0;

	if (Kif::IsSankaku(str[0]))
	{
		if (str[0] == L'△' || str[0] == L'▽')
		{
			color = WHITE;
		}
		else
		{
			color = BLACK;
		}

		++ofs;
	}

	move.Init();

	std::wstring move_str = str.substr(ofs);

	move.set_side(color);

	if (Kif::ResultFromWStringTable.count(move_str) > 0)
	{
		// 文字列がある場合
		move.set_move_type(Kif::ResultFromWStringTable.at(move_str));
	}
	else if (move_str == L"パス")
	{
		move.set_move_type(MoveType::PASS);
	}
	else 
	{
		move.set_move_type(MoveType::NORMAL);

		if ((str.length() - ofs) < 4)
		{
			return false;
		}

		if (str[ofs] == L'同')
		{
			// 同ほげの場合　前回指し手のtoを使う
			// move.set_move_type(MoveType(move.move_type() | MoveType::SAME_FLAG);
			move.set_to(notation.move_current().to());
			ofs++;
		}
		else
		{
			int suji = Kif::NumberFromWChar(str[ofs]);
			int dan = Kif::NumberFromWChar(str[ofs + 1]);

			if (!(suji >= 1 && suji <= 9))
			{
				return false;
			}

			if (!(dan >= 1 && dan <= 9))
			{
				return false;
			}

			move.set_to(make_square(file_from_suji(suji), rank_from_dan(dan)));
			ofs += 2;
		}

		// スペースのスキップ
		for (;ofs < str.length() && ((str[ofs] == ' ' || str[ofs] == L'　')); ofs++);

		if (ofs >= str.length()) return false;

		// 駒取得
		Piece piece = Kif::PieceFromWStr(str, &ofs);
		if (piece == Piece::NO_PIECE)
		{
			return false;
		}

		if (color == WHITE)
		{
			piece = opp(piece);
		}
		move.set_piece(piece);


		if (ofs >= str.length())
		{
			return false;
		}


		if (str[ofs] == L'成')
		{
			move.set_move_type( MoveType(move.move_type() | PROMOTION_FLAG) );
			ofs++;
		}
		else if (str[ofs] == L'打')
		{
			move.set_move_type(MoveType::DROP);
			ofs++;
		}

		if (!(move.move_type() & MoveType::DROP_FLAG))
		{
			// 打ち以外　移動元の取得

			// 空白スキップ
			for (;ofs < str.length() && (str[ofs] == ' ' || str[ofs] == L'　' || str[ofs] == '('); ofs++);
			
			if ((ofs+2) > str.length()) return false;

			int suji = Kif::NumberFromWChar(str[ofs]);

			int dan = Kif::NumberFromWChar(str[ofs + 1]);
			if (!(suji >= 1 && suji <= 9))
			{
				return false;
			}

			if (!(dan >= 1 && dan <= 9))
			{
				return false;
			}

			move.set_from(make_square(file_from_suji(suji), rank_from_dan(dan)));

			ofs += 2;
		}
	}

	return true;
}

/*-----------------------------------------------------------------------------*/
/**
* @brief wchar文字列をSJISに変換して書き込み
* @param wr 出力ストリーム
* @param wstr 出力文字列
*/
/*-----------------------------------------------------------------------------*/
static void Write(std::ostream& wr, const std::wstring& wstr)
{
	std::string str = StringUtil::ConvertStringFromWString(wstr);
	wr << str;
}

static void Write(std::ostream& wr, const wchar_t* wstr)
{
	std::string str = StringUtil::ConvertStringFromWString(wstr);
	wr << str;
}

/*-----------------------------------------------------------------------------*/
/**
* @brief wchar文字列をSJISに変換して改行付きで書き込み
* @param wr 出力ストリーム
* @param wstr 出力文字列
*/
/*-----------------------------------------------------------------------------*/
static void WriteLine(std::ostream& wr, const std::wstring& wstr)
{
	std::string str = StringUtil::ConvertStringFromWString(wstr);
#ifdef _WIN32
	wr << str << std::endl;
#else
	wr << str << "\r" << "\n";
#endif
}

static void WriteLine(std::ostream& wr, const wchar_t* wstr)
{
	std::string str = StringUtil::ConvertStringFromWString(wstr);
#ifdef _WIN32
	wr << str << std::endl;
#else
	wr << str << "\r" << "\n";
#endif
}


