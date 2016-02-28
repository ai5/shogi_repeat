#include "PipeStream.h"

#include <iostream>

#define ERROR_LOG(msg, x) std::cerr << __FILE__ << " : " << __LINE__ << " : " << (msg) << (x) << std::endl

#ifndef _WIN32
#define CloseHandle(handle) close(handle)
#endif

/*-----------------------------------------------------------------------------*/
/**
 * @brief コンストラクタ
 */
/*-----------------------------------------------------------------------------*/
OPipeStream::OPipeStream()
{
}

OPipeStream::~OPipeStream()
{
	this->Close();
}

/*-----------------------------------------------------------------------------*/
/**
 * @brief Open
 * @note  
 */
/*-----------------------------------------------------------------------------*/
bool OPipeStream::Open()
{
	std::unique_lock<std::mutex> lock(this->mtx_); // lock_guardを使うとこのスコープ内がロックされる

	bool ret = true;
#ifdef _WIN32
	HANDLE writeTemp;

	SECURITY_ATTRIBUTES sa;
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;

	if (!CreatePipe(&this->h_read_, &writeTemp, &sa, 0))
	{
		ERROR_LOG("CreatePipe error = ", GetLastError());
		ret = false;
	}

	if (!DuplicateHandle(GetCurrentProcess(), writeTemp,
		GetCurrentProcess(),
		&this->h_write_, // Address of new handle.
		0, FALSE, // Make it uninheritable.
		DUPLICATE_SAME_ACCESS))
	{
		ERROR_LOG("DuplicateHandle error = ", GetLastError());
		ret = false;
	}

	return ret;
#else
	int pip[2];

	if (pipe(pip) != 0)
	{
		ERROR_LOG("CreatePipe error = ", errno);
		ret = false;
	}
	else
	{
		this->h_read_ = pip[0];
		this->h_write_ = pip[1];
	}

#endif
}

/*-----------------------------------------------------------------------------*/
/**
* @brief close
* @note
*/
/*-----------------------------------------------------------------------------*/
void OPipeStream::Close()
{
	std::unique_lock<std::mutex> lock(this->mtx_); // lock_guardを使うとこのスコープ内がロックされる

	if (this->h_read_ != INVALID_HANDLE_VALUE)
	{
		CloseHandle(this->h_read_);
		this->h_read_ = INVALID_HANDLE_VALUE;
	}

	if (this->h_write_ != INVALID_HANDLE_VALUE)
	{
		CloseHandle(this->h_write_);
		this->h_write_ = INVALID_HANDLE_VALUE;
	}
}

/*-----------------------------------------------------------------------------*/
/**
* @brief 読み出しを閉じる
* @note
*/
/*-----------------------------------------------------------------------------*/
void OPipeStream::CloseRead()
{
	std::unique_lock<std::mutex> lock(this->mtx_); // lock_guardを使うとこのスコープ内がロックされる

	if (this->h_read_ != INVALID_HANDLE_VALUE)
	{
		CloseHandle(this->h_read_);
		this->h_read_ = INVALID_HANDLE_VALUE;
	}
}

/*-----------------------------------------------------------------------------*/
/**
* @brief 書き込み
* @note
*/
/*-----------------------------------------------------------------------------*/
int OPipeStream::Write(const std::string str)
{
	std::unique_lock<std::mutex> lock(this->mtx_); // lock_guardを使うとこのスコープ内がロックされる

#ifdef _WIN32
	DWORD len = -1;

	if (this->h_write_ != INVALID_HANDLE_VALUE)
	{

		if (!WriteFile(this->h_write_, &str[0], str.length(), &len, NULL))
		{
			ERROR_LOG("WriteFile error = ", GetLastError());
		}
	}
#else
	size_t len = -1;

	if (this->h_write_ != INVALID_HANDLE_VALUE)
	{
		len = write(this->h_write_, &str[0], str.length());
	}

#endif
	return len;
}


/*-----------------------------------------------------------------------------*/
/**
* @brief コンストラクタ
*/
/*-----------------------------------------------------------------------------*/
IPipeStream::IPipeStream()
{
}

IPipeStream::~IPipeStream()
{
	this->Close();
}

/*-----------------------------------------------------------------------------*/
/**
* @brief Open
* @note
*/
/*-----------------------------------------------------------------------------*/
bool IPipeStream::Open()
{
	std::unique_lock<std::mutex> lock(this->mtx_); // lock_guardを使うとこのスコープ内がロックされる

	bool ret = true;

#ifdef _WIN32
	HANDLE readTemp;

	SECURITY_ATTRIBUTES sa;
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;

	if (!CreatePipe(&readTemp, &this->h_write_, &sa, 0))
	{
		ret = false;
		ERROR_LOG("CreatePipe error = ", GetLastError());
	}

	if (!DuplicateHandle(GetCurrentProcess(), readTemp,
		GetCurrentProcess(),
		&this->h_read_, // Address of new handle.
		0, FALSE, // Make it uninheritable.
		DUPLICATE_SAME_ACCESS))
	{
		ret = false;
		ERROR_LOG("DuplicateHandle error = ", GetLastError());
	}
#else
	int pip[2];

	if (pipe(pip) != 0)
	{
		ERROR_LOG("CreatePipe error = ", errno);
		ret = false;
	}
	else
	{
		this->h_read_ = pip[0];
		this->h_write_ = pip[1];
	}
#endif
	return ret;
}

/*-----------------------------------------------------------------------------*/
/**
* @brief close
* @note
*/
/*-----------------------------------------------------------------------------*/
void IPipeStream::Close()
{
	std::unique_lock<std::mutex> lock(this->mtx_); // lock_guardを使うとこのスコープ内がロックされる

	if (this->h_read_ != INVALID_HANDLE_VALUE)
	{
		CloseHandle(this->h_read_);
		this->h_read_ = INVALID_HANDLE_VALUE;
	}

	if (this->h_write_ != INVALID_HANDLE_VALUE)
	{
		CloseHandle(this->h_write_);
		this->h_write_ = INVALID_HANDLE_VALUE;
	}

	this->strbuf_ = "";
}

/*-----------------------------------------------------------------------------*/
/**
* @brief 書き込み閉じる
* @note
*/
/*-----------------------------------------------------------------------------*/
void IPipeStream::CloseWrite()
{
	std::unique_lock<std::mutex> lock(this->mtx_); // lock_guardを使うとこのスコープ内がロックされる

	if (this->h_write_ != INVALID_HANDLE_VALUE)
	{
		CloseHandle(this->h_write_);
		this->h_write_ = INVALID_HANDLE_VALUE;
	}
}

/*-----------------------------------------------------------------------------*/
/**
* @brief １行ごとの読み出し
* @note
*/
/*-----------------------------------------------------------------------------*/
bool IPipeStream::ReadLine(std::string& str)
{
	std::unique_lock<std::mutex> lock(this->mtx_); // lock_guardを使うとこのスコープ内がロックされる

	if (this->h_read_ == INVALID_HANDLE_VALUE)
	{
		ERROR_LOG("h_read err = ", this->h_read_);
		return false;
	}

	if (this->eof_flag)
	{
		ERROR_LOG("eof ", this->h_read_);
		return false;
	}

#ifdef _WIN32
	DWORD len;
#else
	size_t len;
#endif

	char* buf = this->buffer_;
	bool ret = true;

	while (1)
	{
		if (this->buffer_pos_ < this->buffer_len_)
		{
			for (size_t i = this->buffer_pos_; i < this->buffer_len_; i++)
			{
				char ch = buf[i];
				if (ch == '\r')
				{
					str = this->strbuf_ + std::string(buf + this->buffer_pos_, i - this->buffer_pos_);
					this->strbuf_ = "";

					if (buf[i + 1] == '\n')
					{
						i++;
					}

					this->buffer_pos_ = i + 1;
					return true;
				}
				else if (ch == '\n')
				{
					str = this->strbuf_ + std::string(buf + this->buffer_pos_, i - this->buffer_pos_);

					this->strbuf_ = "";
					this->buffer_pos_ = i + 1;

					return true;
				}
			}

			// 改行なかた
			this->strbuf_ += std::string(this->buffer_ + this->buffer_pos_, this->buffer_len_ - this->buffer_pos_);
			this->buffer_len_ = 0;
			this->buffer_pos_ = 0;
		}

#ifdef _WIN32
		BOOL err = ReadFile(this->h_read_, this->buffer_, sizeof(this->buffer_) - 1, &len, NULL);
#else
		bool err = true;
		len = read(this->h_read_, this->buffer_, sizeof(this->buffer_) - 1);
#endif
		if (!err || len == 0)
		{
#ifdef _WIN32
			if (ERROR_BROKEN_PIPE != GetLastError())
			{
				// 子プロセス側でパイプが閉じられた（大体においてプロセス終了時）以外のエラーを表示
				ERROR_LOG("ReadFile error = ", GetLastError());
			}
#endif
			this->eof_flag = true; // 読み出し終了

			if (this->strbuf_ == "")
			{
				ret = false;
			}
			else
			{
				str = this->strbuf_;
				this->strbuf_ = "";
			}

			break;
		}

		this->buffer_pos_ = 0;
		this->buffer_len_ = len;
	}

	return ret;
}

