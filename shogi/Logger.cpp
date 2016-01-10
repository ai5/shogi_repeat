#include "Logger.h"

#include <iostream>



Log Logger::log;


/*-----------------------------------------------------------------------------*/
/**
* @brief オープン
* @param filename ファイル名
* @note
*/
/*-----------------------------------------------------------------------------*/

Log::Log()
{
}


Log::~Log()
{
	this->Close();
}

/*-----------------------------------------------------------------------------*/
/**
 * @brief オープン
 * @param filename ファイル名
 * @note  
 */
/*-----------------------------------------------------------------------------*/
void Log::Open(const std::string& filename)
{
	std::unique_lock<std::mutex> lock(this->mtx_);

	if (this->filename_ == filename && this->file_.is_open())
	{
		// ファイル名が同じですでに開いている
		return;
	}
	
	if (this->file_.is_open())
	{
		this->file_.close();
	}

	this->filename_ = filename;

	if (this->output_file_)
	{
		this->file_.open(filename);
	}
}

/*-----------------------------------------------------------------------------*/
/**
* @brief クローズ
* @note
*/
/*-----------------------------------------------------------------------------*/
void Log::Close()
{
	std::unique_lock<std::mutex> lock(this->mtx_);

	if (this->file_.is_open())
	{
		this->file_.close();
	}
	this->filename_ = "";
}

/*-----------------------------------------------------------------------------*/
/**
* @brief 1行書き込み
* @note
*/
/*-----------------------------------------------------------------------------*/
void Log::WriteLine(const std::string& str)
{
	std::unique_lock<std::mutex> lock(this->mtx_);

	if (this->output_stdout_)
	{
		std::cout << str << std::endl;
	}

	if (this->output_file_ && this->file_.is_open())
	{
		this->file_ << str << std::endl;
	}
}
/*-----------------------------------------------------------------------------*/
/**
* @brief 出力切り替え
* @note
*/
/*-----------------------------------------------------------------------------*/
void Log::SetOutputStdout(bool flag)
{
	std::unique_lock<std::mutex> lock(this->mtx_);
	this->output_stdout_ = flag;
}

/*-----------------------------------------------------------------------------*/
/**
* @brief 1行書き込み
* @note
*/
/*-----------------------------------------------------------------------------*/
void Log::SetOutputFile(bool flag)
{
	std::unique_lock<std::mutex> lock(this->mtx_);

	if (this->output_file_ != flag)
	{
		this->output_file_ = flag;

		if (!this->file_.is_open() && this->filename_ != "")
		{
			this->file_.open(this->filename_);
		}
	}
}



