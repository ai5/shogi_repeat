// shogi.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include <iostream>
#include <string>

#include "Game.h"
#include "StringUtil.h"
#include "Kif.h"
#include "Sfen.h"
#include "DateTime.h"

static void usage(const char* argv, GameParam& param);
static void parse_args(int argc, char* argv[], GameParam& param);
static void print_settings(GameParam& param);

static bool InteractiveMode = false;

/*-----------------------------------------------------------------------------*/
/**
 * @note  
 */
/*-----------------------------------------------------------------------------*/
int main(int argc, char* argv[])
{
	GameParam param;

	if (argc < 3)
	{
		usage(argv[0], param);
		return -1;
	}

	parse_args(argc, argv, param);

	print_settings(param);

	Game game;

	game.Start(param);

	if (!InteractiveMode)
	{
		game.Wait(); // 終了待つ
	}
	else
	{
		// キー入力
		std::string cmd;
		do {
			if (!getline(std::cin, cmd))
			{
				cmd = "quit";
			}

			if (cmd == "quit")
			{
				game.Cancel();
			}
			else if (cmd == "stop")
			{
				// 対局を止める処理（未実装
			}
			else if (cmd == "save")
			{
				// 設定の保存
				game.SaveOption(BLACK, "black.ini");
				game.SaveOption(WHITE, "white.ini");
			}

		} while (cmd != "quit");

		game.Terminate();
	}

	return 0;
}

/*-----------------------------------------------------------------------------*/
/**
* @brief usageの表示
*/
/*-----------------------------------------------------------------------------*/
static void usage(const char* name, GameParam& param)
{
	std::cout << name << " <先手エンジン>  <後手エンジン> [オプション]" << std::endl;
	std::cout << "オプション:" << std::endl;

	std::cout << "  -B <設定ファイル>   先手設定フィアル" << std::endl;
	std::cout << "  -W <設定ファイル>   後手設定フィアル" << std::endl;

	std::cout << "  -T <持ち時間(秒)> [秒読み(秒)]  持ち時間設定" << std::endl;
	std::cout << "  -T- <持ち時間(秒)> [秒読み(秒)]  持ち時間設定 & 時間切れ負け無し" << std::endl;

	std::cout << "  -G <num>            対局数" << std::endl;
	std::cout << "  -M <num>            最大手数" << std::endl;

	std::cout << "  -P                  PonderをONにする" << std::endl;
	std::cout << "  -P-                 PonderをOFFにする" << std::endl;
	std::cout << "  -S                  先後入れ替えをONにする" << std::endl;
	std::cout << "  -S-                 先後入れ替えをOFFにする" << std::endl;
	std::cout << "  -L [ログ保存パス]   ログ出力をON、ログ保存パスを設定する" << std::endl;
	std::cout << "  -L-                 ログ出力をOFFにする" << std::endl;
	std::cout << "  -N [棋譜保存パス]   棋譜出力ON、棋譜を保存するパスを設定する" << std::endl;
	std::cout << "  -N-                 棋譜出力OFFにする" << std::endl;
	std::cout << "  -K <棋譜> [手数]    開始局面を指定した棋譜ファイルの手数からにする" << std::endl;

	std::cout << "  -I                  対話モード" << std::endl;

	std::cout << std::endl << "デフォルト値" << std::endl;
	print_settings(param);
}
/*-----------------------------------------------------------------------------*/
/**
* @brief 設定出力
*/
/*-----------------------------------------------------------------------------*/
static void print_settings(GameParam& param)
{
	std::cout << "先手:" << param.Black.FileName  << std::endl;
	std::cout << "持ち時間:" << DateTime::ToTimeString(param.Black.time) << "分 秒読み:" << DateTime::ToTimeString(param.Black.byoyomi) << "秒" << std::endl;
	if (param.Black.SettingFileName != "")
	{
		std::cout << "設定ファイル: " << param.Black.SettingFileName << std::endl;
	}

	std::cout << "後手:" << param.White.FileName  << std::endl;
	std::cout << "持ち時間:" << DateTime::ToTimeString(param.White.time) << "分 秒読み:" << DateTime::ToTimeString(param.White.byoyomi) << "秒" << std::endl;
	if (param.White.SettingFileName != "")
	{
		std::cout << "設定ファイル: " << param.White.SettingFileName << std::endl;
	}

	std::cout << std::endl;
	std::cout << "対局数:" << param.MaxPlays << std::endl;
	std::cout << "最大手数:" << param.MaxMoves << std::endl;
	std::cout << "時間切れ負け: " << (param.Timeout ? "ON" : "OFF") << std::endl;
	std::cout << "Ponder: " << (param.PonderON ? "ON" : "OFF") << std::endl;
	std::cout << "先後入れ替え:" << (param.SwapPlayer ? "はい" : "いいえ") << std::endl;
	std::cout << "ログ記録:" << (param.SaveLog ? "はい" : "いいえ") << std::endl;
	if (param.SaveLog)	std::cout << "ログ保存フォルダ:" << (param.LogPath != "" ? param.LogPath : "カレント") << std::endl;
	std::cout << "棋譜保存:" << (param.SaveNotation ? "はい" : "いいえ") << std::endl;
	if (param.SaveLog)	std::cout << "棋譜保存フォルダ:" << (param.KifPath != "" ? param.KifPath : "カレント") << std::endl;

	if (param.UseNotation)
	{
		Notation notation = param.notation;
		if (param.Num >= 0)
		{
			notation.Jump(param.Num);
		}
		else
		{
			notation.Last();
		}

		std::cout << "開始局面:" << std::endl;
		std::cout << "sfen " << Sfen::PositionToString(notation.position(), notation.number() + 1);
	}
	else
	{
		std::cout << "開始局面:" << HandicapExtention::ToString(param.handicap) << std::endl;
	}
}

/*-----------------------------------------------------------------------------*/
/**
* @brief コマンド・ライン引数の処理
*/
/*-----------------------------------------------------------------------------*/
static void parse_args(int argc, char* argv[], GameParam& param)
{
	param.Black.FileName = argv[1];
	param.White.FileName = argv[2];

	for (int i = 3; i < argc; i++)
	{
		const char* arg = argv[i];

		if (arg[0] == '-')
		{
			switch (toupper(arg[1]))
			{
			case 'B':
				if ((i + 1) < argc)
				{
					param.Black.SettingFileName = argv[i + 1];
					i++;
				}
				break;
			case 'W':
				if ((i + 1) < argc)
				{
					param.White.SettingFileName = argv[i + 1];
					i++;
				}
				break;
			case 'T':
				param.Timeout = (arg[2] == '-') ? false : true;
				// 持ち時間設定
				if ((i + 1) < argc)
				{
					int num;
					
					num = StringUtil::ParseTimeMs<std::string>(argv[i + 1]);

					param.Black.time = num;
					param.White.time = num;

					i++;
				}

				if ((i + 1) < argc && argv[i + 1][0] != '-')
				{
					int byoyomi;

					byoyomi = StringUtil::ParseTimeMs<std::string>(argv[i + 1]);

					param.Black.byoyomi = byoyomi;
					param.White.byoyomi = byoyomi;

					i++;
				}

				break;
			case 'G':
				if ((i + 1) < argc)
				{
					int num;

					if (StringUtil::ParseNum<std::string>(argv[i + 1], num))
					{
						param.MaxPlays = num;
					}

					i++;
				}
				break;
			case 'P':
				param.PonderON = (arg[2] == '-') ? false : true;
				break;
			case 'S':
				param.SwapPlayer = (arg[2] == '-') ? false : true;
				break;
			case 'L':
				param.SaveLog = (arg[2] == '-') ? false : true;
				if ((i + 1) < argc)
				{
					if (argv[i + 1][0] != '-')
					{
						i++;
						param.LogPath = argv[i];
					}
				}
				break;
			case 'N':
				param.SaveNotation = (arg[2] == '-') ? false : true;
				if ((i + 1) < argc)
				{
					if (argv[i + 1][0] != '-')
					{
						i++;
						param.KifPath = argv[i];
					}
				}
				break;
			case 'M':
				if ((i + 1) < argc)
				{
					int num;

					if (StringUtil::ParseNum<std::string>(argv[i + 1], num))
					{
						param.MaxMoves= num;
					}

					i++;
				}
				break;
			case 'K':
				if ((i + 1) < argc)
				{
					Kif kif;

					if (kif.Load(param.notation, argv[i + 1]))
					{
						param.UseNotation = true;
					}

					i++;
				}

				if ((i + 1) < argc && argv[i+1][0] != '-')
				{

					int num;

					if (StringUtil::ParseNum<std::string>(argv[i + 1], num))
					{
						param.Num = num;
					}

					i++;
				}

				break;
			case 'I':
				InteractiveMode = (arg[2] == '-') ? false : true;;
				break;
			}
		}
	}
}



