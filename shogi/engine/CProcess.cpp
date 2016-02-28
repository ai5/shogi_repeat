
#include <iostream>

#include "CProcess.h"
#include "Path.h"

#ifndef _WIN32
 #include <sys/wait.h>
 #include <sys/types.h>
 #include <signal.h>
#endif


#define ERROR_LOG(msg, x) std::cerr << __FILE__ << " : " << __LINE__ << " : " << (msg) << (x) << std::endl


Process::Process()
{
}


Process::~Process()
{
	this->Close();
}


/*-----------------------------------------------------------------------------*/
/**
 * @brief プロセス開始
 * @param 
 * @note  
 */
/*-----------------------------------------------------------------------------*/
bool Process::Start()
{
	std::unique_lock<std::mutex> lock(this->mtx_);

	if (this->hChildProcess != INVALID_HANDLE_VALUE)
	{
		return false; // ばかよけ
	}
#ifdef _WIN32
	STARTUPINFO si;
	memset(&si, 0, sizeof(si));
	si.cb = sizeof(si);

	PROCESS_INFORMATION  pi;
	memset(&pi, 0, sizeof(pi));


	DWORD dwCreationFlags = CREATE_NEW_CONSOLE;
	if (this->StartInfo.CreateNoWindow) 
	{
		// どっち使うの？
		// si.wShowWindow |= SW_HIDE;
		dwCreationFlags |= CREATE_NO_WINDOW;
	}

	si.dwFlags |= STARTF_USESTDHANDLES;

	
	// Std out
	if (this->StartInfo.RedirectStandardOutput)
	{
		this->StdOut.Open();
		si.hStdOutput = this->StdOut.WrteHandle();
	}
	else
	{
		si.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE); // 親プロセスの標準出力を継承
	}

	// STD Error
	if (this->StartInfo.RedirectStandardError)
	{
		this->StdErr.Open();
		si.hStdError = this->StdErr.WrteHandle();
	}
	else
	{
		si.hStdError = GetStdHandle(STD_OUTPUT_HANDLE); // 親プロセスの標準出力を継承
	}

	// STD IN
	if (this->StartInfo.RedirectStandardInput)
	{
		this->StdIn.Open();
		si.hStdInput = this->StdIn.ReadHandle();
	}
	
	
	BOOL ret = CreateProcess(
		NULL,
		&this->StartInfo.FileName[0],
		NULL,
		NULL,
		TRUE,
		dwCreationFlags,
		NULL,
		(this->StartInfo.WorkingDirectory != "") ? &this->StartInfo.WorkingDirectory[0] : NULL, // fullpath?
		&si,
		&pi);

	if (!ret)
	{
		ERROR_LOG("CreateProcess error = ", GetLastError());
	}
	
	this->StdOut.CloseWrite(); 
	this->StdErr.CloseWrite();
	this->StdIn.CloseRead();

	// サンプルではこのハンドルド閉じてるけど意味わからん
	if (!CloseHandle(pi.hThread)) { 
		ERROR_LOG("CloseHandle error = ", GetLastError());
	}

	this->hChildProcess = pi.hProcess;

	this->exit_thread_ = std::make_unique<std::thread>(&Process::exitThread, this);
	return ret != FALSE;
#else
	bool ret = true;

	// Std out
	if (this->StartInfo.RedirectStandardOutput)
	{
		this->StdOut.Open();
	}

	// STD Error
	if (this->StartInfo.RedirectStandardError)
	{
		this->StdErr.Open();
	}

	// STD IN
	if (this->StartInfo.RedirectStandardInput)
	{
		this->StdIn.Open();
	}


	int pid = fork();
	if (pid == -1)
	{
		ret = false;
		ERROR_LOG("fork error = ", errno);
	}
	else if (pid == 0)
	{
		// 子プロセス
		if (this->StartInfo.RedirectStandardOutput)
		{
			close(1);
			dup(this->StdOut.WrteHandle());
			this->StdOut.Close();
		}

		if (this->StartInfo.RedirectStandardError)
		{
			close(2);
			dup(this->StdErr.WrteHandle());
			this->StdErr.Close();
		}

		if (this->StartInfo.RedirectStandardInput)
		{
			close(0);
			dup(this->StdIn.ReadHandle());
			this->StdIn.Close();
		}

		if (this->StartInfo.WorkingDirectory != "")
		{
			// カレントディレクトリを変更
			chdir(this->StartInfo.WorkingDirectory.c_str());
		}

		execlp(this->StartInfo.FileName.c_str(), Path::FileName(this->StartInfo.FileName).c_str(), NULL);
		ERROR_LOG("execlp error ", this->StartInfo.FileName);

		exit(EXIT_FAILURE);
	}
	else
	{
		this->hChildProcess = pid;

		// 使用しない側のハンドルを閉じる
		this->StdOut.CloseWrite();
		this->StdErr.CloseWrite();
		this->StdIn.CloseRead();

		this->exit_thread_ = std::make_unique<std::thread>(&Process::exitThread, this);
	}

	return ret;
#endif
}

/*-----------------------------------------------------------------------------*/
/**
* @brief プロセス終了待ち
* @param timeout msec
* @note
*/
/*-----------------------------------------------------------------------------*/
void Process::WaitForExit(int timeout)
{
	HANDLE handle = this->hChildProcess;
#ifdef _WIN32
	if (handle != INVALID_HANDLE_VALUE)
	{
		if (timeout < 0)
		{
			WaitForSingleObject(handle, INFINITE);
		}
		else
		{
			WaitForSingleObject(handle, timeout);
		}
	}
#else
	if (handle != INVALID_HANDLE_VALUE)
	{
		if (timeout >= 0)
		{
			// タイムアウト
			// Linuxはタイムアウトどうやるの？ 別スレッドでタイムアウト見てプロセスKill?
		}
		
		int status;
		do
		{
			pid_t w = waitpid(this->hChildProcess, &status, WUNTRACED | WCONTINUED);
			if (w == -1)
			{
				break;
			}

			if (WIFEXITED(status))
			{
				// ERROR_LOG("exited, status=", WEXITSTATUS(status));
			}
			else if (WIFSIGNALED(status)) {
				ERROR_LOG("killed by signal ", WTERMSIG(status));
			}
			else if (WIFSTOPPED(status)) {
				ERROR_LOG("stopped by signal ", WSTOPSIG(status));
			}
			else if (WIFCONTINUED(status)) {
				ERROR_LOG("continued", 0);
			}
		} while (!WIFEXITED(status) && !WIFSIGNALED(status));
	}
#endif
}


/*-----------------------------------------------------------------------------*/
/**
* @brief ハンドル閉じる
* @note
*/
/*-----------------------------------------------------------------------------*/
void Process::Close()
{
	bool join = false;
	{
		std::unique_lock<std::mutex> lock(this->mtx_);
		if (this->hChildProcess != INVALID_HANDLE_VALUE)
		{

			this->exited_ = nullptr;
#ifdef _WIN32
			TerminateProcess(this->hChildProcess, 0);

			CloseHandle(this->hChildProcess);
#else
			// Linuxはどうする？
			// waitで終了しているはず？
			kill(this->hChildProcess, SIGTERM);
#endif
			this->hChildProcess = INVALID_HANDLE_VALUE;

			// プロセスのハンドルを閉じてから、パイプのハンドルを閉じる
			this->StdIn.Close();
			this->StdErr.Close();
			this->StdOut.Close();
			
			join = true;
		}
	}

	if (join)
	{
		// 一応終了を待つ
		this->exit_thread_->join();
	}
}

/*-----------------------------------------------------------------------------*/
/**
* @brief Exitコールバック設定 
* @param
* @note コールバックは別スレッドから呼び出されることに注意
*/
/*-----------------------------------------------------------------------------*/
void Process::SetExited(std::function<void()> func)
{
	std::unique_lock<std::mutex> lock(this->mtx_);

	this->exited_ = func;
}

/*-----------------------------------------------------------------------------*/
/**
* @brief Exitの監視
* @note processの終了を検知したらコールバック関数を呼び出す
*/
/*-----------------------------------------------------------------------------*/
void Process::exitThread()
{
#ifdef _WIN32
	WaitForSingleObject(this->hChildProcess, INFINITE);
#else
	int status;
	do 
	{
		pid_t w = waitpid(this->hChildProcess, &status, WUNTRACED | WCONTINUED);
		if (w == -1)
		{
			break;
		}

	} while (!WIFEXITED(status) && !WIFSIGNALED(status));
#endif
	{
		std::unique_lock<std::mutex> lock(this->mtx_);

		if (this->exited_ != nullptr)
		{
			this->exited_(); // callback内からProcess関連の処理を呼び出すと死ぬが、そんなことしないよね？
		}
	}

}
