#include "Sfen.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include <cctype>

#include "types.h"
#include "Notation.h"
#include "Position.h"
#include "Tokenizer.h"
#include "MovesEnumerator.h"
#include "StringUtil.h"

/*-----------------------------------------------------------------------------*/

const PieceType Sfen::handPieceType[] = {
	ROOK, BISHOP, GOLD, SILVER, KNIGHT, LANCE, PAWN
};


/*-----------------------------------------------------------------------------*/
/**
*/
/*-----------------------------------------------------------------------------*/

Sfen::Sfen()
{
}


Sfen::~Sfen()
{
}

/// <summary>
/// Sfen棋譜判定
/// </summary>
/// <param name="str">判定文字列</param>
/// <returns>true Sfen棋譜</returns>
/*-----------------------------------------------------------------------------*/
/**
 * @brief Sfen棋譜判定
 * @param 文字列
 * @return true Sfen棋譜 
 * @note  
 */
/*-----------------------------------------------------------------------------*/
bool Sfen::CanRead(const std::string& str)
{
	return Sfen::IsSfen(str);
}


bool Sfen::IsSfen(std::string str)
{
	bool ret = false;

	if (StringUtil::StartsWith(str, "position") || StringUtil::StartsWith(str, "sfen") || StringUtil::StartsWith(str, "startpos") || StringUtil::StartsWith(str, "moves"))
	{
		ret = true;
	}

	return ret;
}

/*-----------------------------------------------------------------------------*/
/**
@brief ファイルから読み込み
@param notation 棋譜クラス
@param filename ファイル名
@note sfen形式の棋譜を読み込む
*/
/*-----------------------------------------------------------------------------*/
void Sfen::Load(Notation& notation, const std::string& filename)
{
	std::ifstream file;
	std::string sfen;

	notation.Init();

	file.open(filename.c_str());

	if (!file.is_open()) {
		std::cerr << "file open error" << filename << std::endl;
		return;
	}

	if (!file.eof())
	{
		std::getline(file, sfen);
		Sfen::ReadNotation(notation, sfen);
	}

	file.close();
}

/*-----------------------------------------------------------------------------*/
/**
@brief ファイルに保存
@param notation 棋譜クラス
@param filename ファイル名
@note sfen形式でファイルに保存する。
*/
/*-----------------------------------------------------------------------------*/
void Sfen::Save(Notation& notation, const std::string& filename)
{
	std::ofstream file;

	file.open(filename.c_str());

	if (!file.is_open()) {
		std::cerr << "file open error" << filename << std::endl;
		return;
	}

	Sfen::WriteNotation(notation, file);


	file.close();
}


/*-----------------------------------------------------------------------------*/
/**
 * @brief 文字列から読み込み
 * @param notation 棋譜クラス
 * @param sfen　USI形式の棋譜
 * @note  
 */
/*-----------------------------------------------------------------------------*/
void Sfen::LoadFromString(Notation& notation, const std::string& sfen)
{
	notation.Init();

	Sfen::ReadNotation(notation, sfen);
}


/*-----------------------------------------------------------------------------*/
/**
@brief USI棋譜を返す
@param notation 棋譜クラス
@return USI棋譜文字列
*/
/*-----------------------------------------------------------------------------*/
std::string Sfen::SaveToString(const Notation& notation)
{
	std::stringstream ss;


	Sfen::WriteNotation(notation, ss);


	return ss.str();
}

/*-----------------------------------------------------------------------------*/
/**
@brief 棋譜読み込み
@param notation 棋譜クラス
@param sfen 棋譜
@note
*/
/*-----------------------------------------------------------------------------*/
void Sfen::LoadNotation(Notation& notation, const std::string& sfen)
{
	notation.Init();

	Sfen::ReadNotation(notation, sfen);
}


/*-----------------------------------------------------------------------------*/
/**
@brief 局面文字列のロード
@param [out] position ポジションクラス
@param sfen 局面情報
@note
*/
/*-----------------------------------------------------------------------------*/
void Sfen::LoadPosition(Position& position, const std::string& sfen)
{
	position.Init();
	Sfen::ReadPosition(position, sfen);
	position.InitHashKey();
}

/*-----------------------------------------------------------------------------*/
/**
 * @brief Position情報の文字列を返す
 * @param psition ポジションクラス
 * @param num n手目 + 1
 * @return 局面文字列
 * @note  "sfen"は付きません
 */
/*-----------------------------------------------------------------------------*/
std::string Sfen::PositionToString(const Position& position, int num)
{
	std::stringstream ss;

	Sfen::WritePosition(position, ss, 1);

	return ss.str();
}

/*-----------------------------------------------------------------------------*/
/**
* @brief カレント位置までの指し手を返す
* @param notation 棋譜クラス
* @return 指し手文字列
* @note  "moves"は付きません
*        用途的にresultは不要なのでresultは出力しない
*/
/*-----------------------------------------------------------------------------*/
std::string Sfen::MovesToString(const Notation& notation)
{
	std::stringstream ss;

	Sfen::WriteMoves(notation, ss, notation.number());

	return ss.str();
}

/*-----------------------------------------------------------------------------*/
/**
* @brief 指し手文字列を返す
* @param notation 棋譜クラス
* @return 指し手文字列
* @note  "moves"は付きません
*/
/*-----------------------------------------------------------------------------*/
std::string Sfen::MoveToString(const Move& move)
{
	std::stringstream ss;

	Sfen::WriteMove(move, ss);

	return ss.str();
}


/*-----------------------------------------------------------------------------*/
/**
@brief usi形式の棋譜読み込み
*/
/*-----------------------------------------------------------------------------*/
void Sfen::ReadNotation(Notation& notation, const std::string& sfen)
{
	Tokenizer tok(sfen, " \n\r");

	std::string token;

	token = tok.GetToken();
	if (token == "position")
	{
		token = tok.GetToken();
	}

	if (token == "startpos")
	{
		// 処理無し
	}
	else if (token == "sfen")
	{
		// positionの処理
		Position pos;
		Sfen::ReadPosition(pos, tok.GetToken("moves"));
		notation.SetInitialPosition(pos);
	}
	else if (token == "moves")
	{
		// 次で処理する
		tok.Previous();
	}

	token = tok.GetToken();
	if (token == "moves")
	{
		// 指し手の処理
		while ((token = tok.GetToken()) != "")
		{
			Move move = Sfen::ParseMove(notation.position(), token);
			if (move.IsNoMove())
			{
				// 指し手が変だべよ
				break;
			}

			// 合法手チェック(未実装

			// 棋譜に指し手を追加
			notation.AddMove(MoveKif(move));
		}
	}
}

/*-----------------------------------------------------------------------------*/
/**
@brief usi形式のポジション 読み込み
*/
/*-----------------------------------------------------------------------------*/
void Sfen::ReadPosition(Position& pos, const std::string& sfen)
{
	Tokenizer tok(sfen, " \n\r");

	File file;
	Rank rank;


	pos.BoardClear();

	std::string token;

	token = tok.GetToken();

	// 盤面読み込み
	std::string::iterator ite = token.begin();
	file = FILE_9;
	rank = RANK_1;

	while (ite != token.end())
	{
		char ch = *ite++;

		if (ch == '/')
		{
			// 段の区切り
			file = FILE_9;
			rank += 1;
			if (rank >= Rank::RANK_NB)
			{
				break;
			}
		}
		else if (ch >= '0' && ch <= '9')
		{
			// 数字は駒がないところ
			file += ch - '0';
		}
		else
		{
			bool promotion = false;

			// なり
			if (ch == '+')
			{
				promotion = true;

				if (ite  != token.end())
				{
					ch = *ite++;
				}
				else
				{
					break;
				}
			}

			// 後手
			Color piece_color = BLACK;
			PieceType piece_type = NO_PIECE_TYPE;

			if (ch >= 'a' && ch <= 'z')
			{
				piece_color = WHITE;
				ch = toupper(ch);
			}

			piece_type = Sfen::ConvPieceTypeFromChar(ch);

			Piece piece = make_piece(piece_color, piece_type, promotion);


			if (piece_type != PieceType::NO_PIECE_TYPE)
			{
				// 駒がある
				if (file < File::FILE_NB)
				{
					pos.SetPiece(file, rank, piece);
					file += 1;
				}
			}
		}
	}

	// 手番
	token = tok.GetToken();
	if (token == "w")
	{
		pos.set_side_to_move(WHITE);
	}
	else if (token == "b")
	{
		pos.set_side_to_move(BLACK);
	}



	// 持ち駒
	token = tok.GetToken();

	ite = token.begin();
	while (ite != token.end())
	{
		char ch = *ite++;

		int piece_num = 1;

		// 枚数
		if (ch >= '0' && ch <= '9')
		{
			piece_num = ch - '0';
			if (ite == token.end())
			{
				break;
			}

			ch = *ite++;

			if (ch >= '0' && ch <= '9')
			{
				piece_num = (piece_num * 10) + (ch - '0');
				if (ite == token.end())
				{
					break;
				}

				ch = *ite++;
			}
		}

		if (isupper(ch))
		{
			// 大文字の場合
			PieceType piece_type = Sfen::ConvPieceTypeFromChar(ch);
			pos.SetBlackHand(piece_type, piece_num);
		}
		else
		{
			PieceType piece_type = Sfen::ConvPieceTypeFromChar(toupper(ch));
			pos.SetWhiteHand(piece_type, piece_num);
		}		
	}

	// n手目 (読み飛ばす
	token = tok.GetToken();
}

/*-----------------------------------------------------------------------------*/
/**
@brief usi 指し手を Moveに変換する
*/
/*-----------------------------------------------------------------------------*/
Move Sfen::ParseMove(const Position& position, const std::string& move)
{
	if (move == "resign")
	{
		return Move(MoveType::RESIGN); // 投了
	}
	else if (move == "win")
	{
		// 反則勝ち or 入玉勝ち
		return Move(MoveType::NYUGYOKU_KACHI); // 入玉勝ちにしとく
	}
	else if (move == "draw")
	{
		return Move(MoveType::DRAW); // 持将棋
	}
	else if (move == "pass" || move == "0000")
	{
		// uci的には0000でgpsではpassもパスとなっている
		return Move(MoveType::PASS);
	}

	if (move.length() < 4)
	{
		return Move();
	}

	Move movedata;

	if (move[1] == '*')
	{
		// 打つ手
		movedata.set_move_type(MoveType::DROP);

		PieceType piece_type = Sfen::ConvPieceTypeFromChar(move[0]);

		File file = Sfen::FileFromChar(move[2]);
		Rank rank = Sfen::RankFromChar(move[3]);

		if (file == FILE_NB || rank == RANK_NB)
		{
			return Move();
		}

		movedata.set_to(make_square(file, rank));
		movedata.set_piece(make_piece(position.side_to_move(), piece_type));
	}
	else
	{
		// 移動
		movedata.set_move_type(MoveType::NORMAL);

		// from
		File file = Sfen::FileFromChar(move[0]);
		Rank rank = Sfen::RankFromChar(move[1]);

		movedata.set_from(make_square(file, rank));

		file = Sfen::FileFromChar(move[2]);
		rank = Sfen::RankFromChar(move[3]);
		if (file == FILE_NB || rank == RANK_NB)
		{
			return Move();
		}

		movedata.set_to(make_square(file, rank));
		movedata.set_piece(position.GetPiece(movedata.from()));
		
		if (move.length() >= 5 && move[4] == '+')
		{
			// 成り
			movedata.set_move_type( MoveType(movedata.move_type() | MoveType::PROMOTION_FLAG) );
		}

		Piece capture_piece = position.GetPiece(movedata.to());
		if (capture_piece != NO_PIECE)
		{
			// 捕獲する場合
			movedata.set_capture_piece(capture_piece);
			movedata.set_move_type(MoveType(movedata.move_type() | MoveType::CAPTURE_FLAG));
		}
	}

	movedata.set_side(position.side_to_move());

	return movedata;
}

/*-----------------------------------------------------------------------------*/
/**
@brief ストリームにUSI形式の棋譜を出力する 
@param notation 棋譜クラス
@param wr outストリーム
*/
/*-----------------------------------------------------------------------------*/
void Sfen::WriteNotation(const Notation& notation, std::ostream& wr)
{
	wr << "position ";
	if (notation.handicap() == Handicap::HIRATE)
	{
		wr << "startpos";
	}
	else
	{
		wr << "sfen ";
		Sfen::WritePosition(notation.initial_position(), wr, 1);
	}

	wr << " moves ";

	Sfen::WriteMoves(notation, wr);
}

/*-----------------------------------------------------------------------------*/
/**
@brief ストリームにUSI形式の棋譜を出力する
@param notation 棋譜クラス
@param wr outストリーム
*/
/*-----------------------------------------------------------------------------*/
void Sfen::WritePosition(const Position& pos, std::ostream& wr, int number)
{
	int space = 0;

	// 盤面出力

	for (Rank rank = RANK_1; rank < Rank::RANK_NB; rank += 1)
	{
		// 段の切り替わりで/を出力
		if (rank != RANK_1)
		{
			wr << '/';
		}

		for (File file = FILE_9; file < File::FILE_NB; file += 1)
		{
			Piece piece = pos.GetPiece(file, rank);
			char ch;

			if (piece == Piece::NO_PIECE)
			{
				space++;
			}
			else
			{
				if (space != 0)
				{
					wr << space;
					space = 0;
				}

				if (is_promoted(piece))
				{
					// 成り
					wr << '+';
				}

				ch = Sfen::ConvCharFromPieceType(type_of(piece));

				if (is_white(piece))
				{
					ch = tolower(ch);
				}

				wr << ch;
			}
		}

		if (space != 0)
		{
			wr << space;
			space = 0;
		}
	}

	// 手番の出力
	if (pos.side_to_move() == WHITE)
	{
		wr << " w ";
	}
	else
	{
		wr << " b ";
	}

	// 持ち駒の出力
	int hand_cnt = 0;

	for (auto pt : Sfen::handPieceType)
	{
		int num = pos.hand()[BLACK][pt];

		if (num != 0)
		{
			if (num > 1)
			{
				wr << num;
			}

			wr << Sfen::ConvCharFromPieceType(pt);

			hand_cnt++;
		}
	}

	for (auto pt : Sfen::handPieceType)
	{
		int num = pos.hand()[WHITE][pt];
		if (num != 0)
		{
			if (num > 1)
			{
				wr << num;
			}

			char ch = Sfen::ConvCharFromPieceType(pt);
			wr <<  tolower(ch); // 後手は小文字

			hand_cnt++;
		}
	}

	if (hand_cnt == 0)
	{
		// 持ち駒ない場合
		wr << "-";
	}

	// 局面の番号 + 1を出力
	if (number != 0)
	{
		wr << number;
	}
}

/*-----------------------------------------------------------------------------*/
/**
@brief ストリームに指し手を出力
@param notation 棋譜クラス
@param wr outストリーム
*/
/*-----------------------------------------------------------------------------*/
void Sfen::WriteMoves(const Notation& notation, std::ostream& wr)
{
	bool first = true;

	for (MovesEnumerator moves(notation.moves()); !moves.IsEnd(); moves.Next())
	{
		const Move& move = moves.Current();

		if (is_move(move.move_type()) || is_result(move.move_type()))
		{
			if (first)
			{
				first = false;
			}
			else
			{
				wr << ' ';
			}

			Sfen::WriteMove(move, wr); 
		}
	}
}


/*-----------------------------------------------------------------------------*/
/**
@brief ストリームに指し手を出力
@param notation 棋譜クラス
@param wr outストリーム
@param number 指し手出力の終了位置
*/
/*-----------------------------------------------------------------------------*/
void Sfen::WriteMoves(const Notation& notation, std::ostream& wr, int number)
{
	bool first = true;

	for (MovesEnumerator moves(notation.moves()); !moves.IsEnd(); moves.Next())
	{
		const MoveKif& move = moves.Current();

		// 指し手のみ出力 resultは出力しない
		if (is_move(move.move_type()))
		{
			if (first)
			{
				first = false;
			}
			else
			{
				wr << ' ';
			}

			Sfen::WriteMove(move, wr);
		}

		if (move.number() == number)
		{
			// 所定の位置で終了
			break;
		}
	}
}

/*-----------------------------------------------------------------------------*/
/**
@brief ストリームに指し手を出力
@param notation 棋譜クラス
@param wr outストリーム
*/
/*-----------------------------------------------------------------------------*/
void Sfen::WriteMove(const Move& move, std::ostream& wr)
{
	// 以下のような感じ
	// ７六歩(77)    7g7f
	// １一1銀成(22) 1a2b+
	// ６五金打      G*6e

	if (is_result(move.move_type()))
	{
		// 結果の場合
		switch (move.move_type())
		{
		case MoveType::RESIGN: // 投了
		case MoveType::TIMEOUT: // 切れ負け
		case MoveType::LOSE_FOUL: // 反則負け
			wr << "resign";
			break;
		case MoveType::REPETITION: // 千日手
		case MoveType::DRAW:  // 持将棋
			wr << "draw";
			break;
		case MoveType::WIN_FOUL: // 反則勝ち
		case MoveType::NYUGYOKU_KACHI: // 入玉勝ち
			wr << "win";
			break;
		default:
			// 上記以外は出力しない
			break;
		}
	}
	else if (move.move_type() == MoveType::PASS)
	{
		wr << "pass"; // gps合わせで0000ではなくpassにする
	}
	else if (move.move_type() & MoveType::DROP_FLAG)
	{
		// 打つ手
		File file = file_of(move.to());
		Rank rank = rank_of(move.to());

		wr << Sfen::ConvCharFromPieceType(type_of(move.piece()))
		   << '*'
		   << (char)('0' + suji_from_file(file))
		   << (char)('a' + rank);
	}
	else
	{
		File to_file = file_of(move.to());
		Rank to_rank = rank_of(move.to());
		File from_file = file_of(move.from());
		Rank from_rank = rank_of(move.from());

		wr << (char)('0' + suji_from_file(from_file))
		   << (char)('a' + from_rank)
		   << (char)('0' + suji_from_file(to_file))
		   << (char)('a' + to_rank);

		if (move.move_type() & MoveType::PROMOTION_FLAG)
		{
			// 成り
			wr << '+';
		}
	}
}

/*-----------------------------------------------------------------------------*/
/**
@brief 文字からpieceTypeを取得する
*/
/*-----------------------------------------------------------------------------*/
PieceType Sfen::ConvPieceTypeFromChar(char ch)
{
	PieceType piece_type = NO_PIECE_TYPE;

	switch (ch) {
	case 'K': piece_type = KING; break;  // 玉 King
	case 'R': piece_type = ROOK; break;  // 飛車 Rook
	case 'B': piece_type = BISHOP; break;  // 角  Bishop
	case 'G': piece_type = GOLD; break;  // 金 Gold
	case 'S': piece_type = SILVER; break;  // 銀 Silver
	case 'N': piece_type = KNIGHT; break;  // 桂馬 kNight
	case 'L': piece_type = LANCE; break;  // 香車 Lance
	case 'P': piece_type = PAWN; break;  // 歩  Pawn
	default:
		//不明な文字列
		break;
	}

	return piece_type;
}

/*-----------------------------------------------------------------------------*/
/**
@brief 文字からpieceTypeを取得する
*/
/*-----------------------------------------------------------------------------*/
char Sfen::ConvCharFromPieceType(PieceType pieceType)
{
	PieceType piece_type = NO_PIECE_TYPE;
	char ch = ' ';

	switch (pieceType) {
	case PAWN:   ch = 'P'; break;
	case LANCE:  ch = 'L'; break;
	case KNIGHT: ch = 'N'; break;
	case SILVER: ch = 'S'; break;
	case GOLD:   ch = 'G'; break;
	case BISHOP: ch = 'B'; break;
	case ROOK:   ch = 'R'; break;
	case KING:   ch = 'K'; break;
	}

	return ch;
}


/*-----------------------------------------------------------------------------*/
/**
@brief 文字からFileを返す
*/
/*-----------------------------------------------------------------------------*/
File Sfen::FileFromChar(char ch)
{
	File file = FILE_NB;

	if (ch >= '1' && ch <= '9')
	{
		int suji = ch - '0';

		file = file_from_suji(suji);
	}

	return file;
}

/*-----------------------------------------------------------------------------*/
/**
 * @brief 文字からRankを返す
 */
/*-----------------------------------------------------------------------------*/
Rank Sfen::RankFromChar(char ch)
{
	Rank rank = RANK_NB;

	if (ch >= 'a' && ch <= 'i')
	{
		rank = Rank(ch - 'a');
	}

	return rank;
}

