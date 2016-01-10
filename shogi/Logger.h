#pragma once

#ifndef SHOGI_LOGGER_H_
#define SHOGI_LOGGER_H_

#include <mutex>
#include <string>
#include <fstream>

class Log
{
	bool output_stdout_ = false;
	bool output_file_ = false;
	
	std::ofstream file_;
	std::string filename_;

	std::mutex mtx_;

public:
	Log();
	~Log();

	void Open(const std::string& filename);
	void Close();

	void WriteLine(const std::string& str);

	void SetOutputStdout(bool flag);
	void SetOutputFile(bool flag);

};


class Logger
{
	static Log log;
public:
	static void Open(const std::string& filename) {	Logger::log.Open(filename);	}
	static void Close() { Logger::log.Close(); }

	static void WriteLine(const std::string& str) { Logger::log.WriteLine(str); }

	static void SetOutputStdout(bool flag) { Logger::log.SetOutputStdout(flag); }
	static void SetOutputFile(bool flag) { Logger::log.SetOutputFile(flag); }

};



#endif

