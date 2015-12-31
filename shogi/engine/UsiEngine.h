#pragma once

#ifndef SHOGI_USI_ENGINE_H_
#define SHOGI_USI_ENGINE_H_

#include <thread>
#include <memory>

#include "USIOption.h"
#include "Process.h"
#include "StringQueue.h"


class USIEngine
{
	USIOptions options_;
	
	std::unique_ptr<Process> process_;
	std::unique_ptr<StringQueue> string_queue_;

	std::unique_ptr<std::thread> stdin_thread_;

	bool Initialized = false;

public:
	USIEngine();
	~USIEngine();

	bool Initialize(const std::string& filename);
	void Terminate();

	STRING_QUEUE_ERR ReadLine(std::string& str, int timeout);
	void WriteLine(const std::string& str);

	/* オプション関連 */
	const USIOptions& options() const { return this->options_; }
	bool HasOption(const std::string& name) const {
		return this->options_.Contents(name);
	}
	std::string GetOptonValue(const std::string& name) const {
		if (this->options_.Contents(name))
		{
			return this->options_.at(name)->ValueToString();
		}

		return "";
	}

	void SetOption(const std::string& name, bool value);
	void SetOption(const std::string& name, int value);
	void SetOption(const std::string& name, const std::string& value);
	bool USIEngine::AddOption(const std::string& str);

	void SetOptions(const std::map<std::string, std::string>& opt_name_value);

protected:
	void stdout_read();

};

#endif

