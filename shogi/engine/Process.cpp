#include <Windows.h>

#include <iostream>

#include "Process.h"


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

	if (this->hChildProcess)
	{
		return false; // ばかよけ
	}

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

	if (handle != 0)
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
		if (this->hChildProcess)
		{

			this->exited_ = nullptr;

			TerminateProcess(this->hChildProcess, 0);

			CloseHandle(this->hChildProcess);
			this->hChildProcess = 0;

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
	WaitForSingleObject(this->hChildProcess, INFINITE);

	{
		std::unique_lock<std::mutex> lock(this->mtx_);

		if (this->exited_ != nullptr)
		{
			this->exited_(); // callback内からProcess関連の処理を呼び出すと死ぬが、そんなことしないよね？
		}
	}
}
