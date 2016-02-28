// child.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#ifdef _WIN32
#include "stdafx.h"
#endif

#include <iostream>
#include <string>

using namespace std;

int main()
{

	string str;
	std::cout << "child process test" << std::endl;

	do
	{
		getline(cin, str);

		cerr << str << endl;

		if (str == "usi")
		{
			cout << "id name child dummy" << endl;
			cout << "id author child" << endl;


			cout << "option name option1 type check default true" << endl;
			cout << "option name My Name type string default My name is hoge" << endl;
			cout << "option name hoge type filename default fname" << endl;

			cout << "usiok" << endl;
		}
		else if (str == "isready")
		{
			cout << "readyok" << endl;
		}
		else if (str.compare(0, 9, "position ") == 0)
		{

		}
		else if (str.compare(0, 3, "go ") == 0)
		{

			cout << "bestmove 7g7f" << endl;
		}
		else if (str.compare(0, 10, "setoption "))
		{
		}
		else if (str == "usinewgame")
		{
		}
		else if (str == "stop")
		{
		}
		else
		{
			cerr << str << endl;
		}

	} while (str != "quit");
	

    return 0;
}

