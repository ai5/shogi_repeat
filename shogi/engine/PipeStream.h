#pragma once

#ifndef SHOGI_PIPE_STREAM_H_
#define SHOGI_PIPE_STREAM_H_

#include <Windows.h>
#include <iostream>
#include <mutex>

// 子プロセスのStdIn用のPipeストリーム
class OPipeStream
{
	std::mutex mtx_;

	HANDLE h_read_;
	HANDLE h_write_;

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

	HANDLE h_read_;
	HANDLE h_write_;

	static const int BUFFER_SIZE = 4096 + 1;

	char buffer_[BUFFER_SIZE];
	uint32_t buffer_pos_;
	uint32_t buffer_len_;

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

