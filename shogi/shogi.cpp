// shogi.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include <iostream>
#include <string>

#include "Game.h"
#include "StringUtil.h"
#include "Kif.h"

static void usage(const char* argv);
static void parse_args(int argc, char* argv[], GameParam& param);


/*-----------------------------------------------------------------------------*/
/**
 * @note  
 */
/*-----------------------------------------------------------------------------*/
int main(int argc, char* argv[])
{

	if (argc < 3)
	{
		usage(argv[0]);
		return -1;
	}

	GameParam param;

	parse_args(argc, argv, param);

	Game game;

	game.Start(param);


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
			
			// msg_que.enqueue(MyMessage(QUIT));
		}
		else if (cmd == "save")
		{
			// 設定の保存
			game.SaveOption(BLACK, "black.ini");
			game.SaveOption(WHITE, "white.ini");
		}

	} while (cmd != "quit");

	game.Terminate();


	return 0;
}

/*-----------------------------------------------------------------------------*/
/**
* @brief usageの表示
*/
/*-----------------------------------------------------------------------------*/
static void usage(const char* name)
{
	std::cout << name << " 先手エンジン  後手エンジン [オプション]" << std::endl;
	std::cout << "オプション:" << std::endl;

	std::cout << "  -B 設定ファイル     先手設定フィアル" << std::endl;
	std::cout << "  -W 設定ファイル     後手設定フィアル" << std::endl;

	std::cout << "  -T 持ち時間(秒) [秒読み(秒)]  持ち時間設定" << std::endl;

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
	std::cout << "  -K 棋譜 [手数]      開始局面を指定した棋譜ファイルの手数からにする" << std::endl;
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
				// 持ち時間設定
				if ((i + 1) < argc)
				{
					int num;
					
					if (StringUtil::ParseNum<std::string>(argv[i + 1], num))
					{
						param.Black.time = num;
						param.White.time = num;
					}

					i++;
				}

				if ((i + 1) < argc && argv[i + 1][0] != '-')
				{
					int byoyomi;

					if (StringUtil::ParseNum<std::string>(argv[i + 1], byoyomi))
					{
						param.Black.byoyomi = byoyomi;
						param.White.byoyomi = byoyomi;
					}

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
			}
		}
	}
}
