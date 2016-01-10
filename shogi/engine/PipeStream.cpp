#include "PipeStream.h"

#include <iostream>

#define ERROR_LOG(msg, x) std::cerr << __FILE__ << " : " << __LINE__ << " : " << (msg) << (x) << std::endl

/*-----------------------------------------------------------------------------*/
/**
 * @brief �R���X�g���N�^
 */
/*-----------------------------------------------------------------------------*/
OPipeStream::OPipeStream()
{
	this->h_read_ = 0;
	this->h_write_ = 0;
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
	std::unique_lock<std::mutex> lock(this->mtx_); // lock_guard���g���Ƃ��̃X�R�[�v�������b�N�����

	HANDLE writeTemp;
	bool ret = true;

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
}

/*-----------------------------------------------------------------------------*/
/**
* @brief close
* @note
*/
/*-----------------------------------------------------------------------------*/
void OPipeStream::Close()
{
	std::unique_lock<std::mutex> lock(this->mtx_); // lock_guard���g���Ƃ��̃X�R�[�v�������b�N�����

	if (this->h_read_ != 0)
	{
		CloseHandle(this->h_read_);
		this->h_read_ = 0;
	}

	if (this->h_write_ != 0)
	{
		CloseHandle(this->h_write_);
		this->h_write_ = 0;
	}
}

/*-----------------------------------------------------------------------------*/
/**
* @brief �ǂݏo�������
* @note
*/
/*-----------------------------------------------------------------------------*/
void OPipeStream::CloseRead()
{
	std::unique_lock<std::mutex> lock(this->mtx_); // lock_guard���g���Ƃ��̃X�R�[�v�������b�N�����

	if (this->h_read_ != 0)
	{
		CloseHandle(this->h_read_);
		this->h_read_ = 0;
	}
}

/*-----------------------------------------------------------------------------*/
/**
* @brief ��������
* @note
*/
/*-----------------------------------------------------------------------------*/
int OPipeStream::Write(const std::string str)
{
	std::unique_lock<std::mutex> lock(this->mtx_); // lock_guard���g���Ƃ��̃X�R�[�v�������b�N�����

	DWORD len = -1;

	if (this->h_write_)
	{

		if (!WriteFile(this->h_write_, &str[0], str.length(), &len, NULL))
		{
			ERROR_LOG("WriteFile error = ", GetLastError());
		}
	}

	return len;
}


/*-----------------------------------------------------------------------------*/
/**
* @brief �R���X�g���N�^
*/
/*-----------------------------------------------------------------------------*/
IPipeStream::IPipeStream()
{
	this->h_read_ = 0;
	this->h_write_ = 0;
	
	this->buffer_pos_ = 0;
	this->buffer_len_ = 0;
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
	std::unique_lock<std::mutex> lock(this->mtx_); // lock_guard���g���Ƃ��̃X�R�[�v�������b�N�����

	bool ret = true;
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
	std::unique_lock<std::mutex> lock(this->mtx_); // lock_guard���g���Ƃ��̃X�R�[�v�������b�N�����

	if (this->h_read_ != 0)
	{
		CloseHandle(this->h_read_);
		this->h_read_ = 0;
	}

	if (this->h_write_ != 0)
	{
		CloseHandle(this->h_write_);
		this->h_write_ = 0;
	}

	this->strbuf_ = "";
}

/*-----------------------------------------------------------------------------*/
/**
* @brief �������ݕ���
* @note
*/
/*-----------------------------------------------------------------------------*/
void IPipeStream::CloseWrite()
{
	std::unique_lock<std::mutex> lock(this->mtx_); // lock_guard���g���Ƃ��̃X�R�[�v�������b�N�����

	if (this->h_write_ != 0)
	{
		CloseHandle(this->h_write_);
		this->h_write_ = 0;
	}
}

/*-----------------------------------------------------------------------------*/
/**
* @brief �P�s���Ƃ̓ǂݏo��
* @note
*/
/*-----------------------------------------------------------------------------*/
bool IPipeStream::ReadLine(std::string& str)
{
	std::unique_lock<std::mutex> lock(this->mtx_); // lock_guard���g���Ƃ��̃X�R�[�v�������b�N�����

	if (!this->h_read_)
	{
		return false;
	}

	if (this->eof_flag)
	{
		return false;
	}

	DWORD len;
	char* buf = this->buffer_;
	bool ret = true;

	while (1)
	{
		if (this->buffer_pos_ < this->buffer_len_)
		{
			for (DWORD i = this->buffer_pos_; i < this->buffer_len_; i++)
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

			// ���s�Ȃ���
			this->strbuf_ += std::string(this->buffer_ + this->buffer_pos_, this->buffer_len_ - this->buffer_pos_);
			this->buffer_len_ = 0;
			this->buffer_pos_ = 0;
		}

		if (!ReadFile(this->h_read_, this->buffer_, sizeof(this->buffer_) - 1, &len, NULL) || len == 0)
		{
			if (ERROR_BROKEN_PIPE != GetLastError())
			{
				// �q�v���Z�X���Ńp�C�v������ꂽ�i��̂ɂ����ăv���Z�X�I�����j�ȊO�̃G���[��\��
				ERROR_LOG("ReadFile error = ", GetLastError());
			}

			this->eof_flag = true; // �ǂݏo���I��

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
