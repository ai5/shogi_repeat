#pragma once

#ifndef SHOGI_PROCESS_H_
#define SHOGI_PROCESS_H_

#include <functional>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include "PipeStream.h"


struct ProcessStartInfo {
	std::string FileName = "";
	bool CreateNoWindow = true;
	bool RedirectStandardInput = false;
	bool RedirectStandardOutput = false;
	bool RedirectStandardError = false;

	std::string WorkingDirectory = "";
};

class Process
{
private:
	HANDLE hChildProcess = 0;
	std::mutex mtx_;
	std::unique_ptr<std::thread> exit_thread_;

	std::function<void()> exited_;
	

public:
	ProcessStartInfo StartInfo;
	OPipeStream StdIn;
	IPipeStream StdOut;
	IPipeStream StdErr;

public:
	Process();
	~Process();


	bool Start();
	void WaitForExit(int timeout);
	void Close();

	void SetExited(std::function<void()> func);

protected:
	void exitThread();

};

#endif