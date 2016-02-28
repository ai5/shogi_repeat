#pragma once

#ifndef SHOGI_PIPE_STREAM_H_
#define SHOGI_PIPE_STREAM_H_

#ifdef _WIN32
 #include <Windows.h>
#else
 #include <unistd.h>
#endif

#include <iostream>
#include <mutex>

#ifndef _WIN32
using HANDLE = int;
static const int INVALID_HANDLE_VALUE = -1;
#endif

// 子プロセスのStdIn用のPipeストリーム
class OPipeStream
{
	std::mutex mtx_;
	HANDLE h_read_ = INVALID_HANDLE_VALUE;
	HANDLE h_write_ = INVALID_HANDLE_VALUE;

public:
	OPipeStream();
	virtual ~OPipeStream();

	bool Open();
	int Write(std::string str);
	void Close();

	void CloseRead();
	HANDLE ReadHandle() const { return this->h_read_; }
};


// 子プロセスのStdOut,StdErr用のPipeストリーム
class IPipeStream
{
	std::mutex mtx_;
	HANDLE h_read_ = INVALID_HANDLE_VALUE;
	HANDLE h_write_ = INVALID_HANDLE_VALUE;
	static const int BUFFER_SIZE = 4096 + 1;

	char buffer_[BUFFER_SIZE];
	uint32_t buffer_pos_ = 0;
	uint32_t buffer_len_ = 0;

	std::string strbuf_;
	bool eof_flag = false;

public:
	IPipeStream();
	virtual ~IPipeStream();

	bool Open();
	void Close();

	void CloseWrite();
	HANDLE WrteHandle() const { return this->h_write_; }
	bool ReadLine(std::string& str);

	bool IsEof() const { return this->eof_flag; }

};


#endif

