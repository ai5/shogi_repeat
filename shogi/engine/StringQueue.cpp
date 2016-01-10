#include "StringQueue.h"



StringQueue::StringQueue()
{
}


StringQueue::~StringQueue()
{
}

/*-----------------------------------------------------------------------------*/
/**
 * @brief キューに詰む
 * @note  
 */
/*-----------------------------------------------------------------------------*/
void StringQueue::Push(const std::string& str)
{
	std::unique_lock<std::mutex> lock(this->mtx_); // lock_guardを使うとこのスコープ内がロックされる
	if (this->close_)
	{
		return;
	}

	this->queue_.push_back(std::make_unique<std::string>(str));

	this->sem_.post();
}

void StringQueue::Push(const char* str)
{
	std::unique_lock<std::mutex> lock(this->mtx_); // lock_guardを使うとこのスコープ内がロックされる
	if (this->close_)
	{
		return;
	}

	if (str != nullptr)
	{
		this->queue_.push_back(std::make_unique<std::string>(str));
	}
	else
	{
		this->queue_.emplace_back(); // 初期要素はnullptr
	}

	this->sem_.post();
}


/*-----------------------------------------------------------------------------*/
/**
* @brief キューに詰む
* @param timeoutMs 最大待ち時間 ms
* @note
*/
/*-----------------------------------------------------------------------------*/
STRING_QUEUE_ERR StringQueue::Pop(std::string& ref_str, int timeoutMs)
{
	if (this->close_)
	{
		return STRING_QUEUE_ERR::CLOSE;
	}

	if (!this->sem_.wait(timeoutMs))
	{
		return STRING_QUEUE_ERR::TIMEOUT;
	}

	STRING_QUEUE_ERR err = STRING_QUEUE_ERR::OK;

	{
		std::unique_lock<std::mutex> lock(this->mtx_); // lock_guardを使うとこのスコープ内がロックされる

		if (this->close_)
		{
			return  STRING_QUEUE_ERR::CLOSE;
		}

		if (&(*this->queue_.front()) == nullptr)
		{
			err = STRING_QUEUE_ERR::ERR;
		}
		else
		{
			ref_str = *this->queue_.front();
		}

		this->queue_.pop_front();
	}

	return err;
}

/*-----------------------------------------------------------------------------*/
/**
* @brief キュー閉じる
* @note
*/
/*-----------------------------------------------------------------------------*/
void StringQueue::Close()
{
	std::unique_lock<std::mutex> lock(this->mtx_); // lock_guardを使うとこのスコープ内がロックされる

	this->close_ = true;

	this->sem_.post();
}