#pragma once
#ifndef SHOGI_MESSAGE_QUEUE_H_
#define SHOGI_MESSAGE_QUEUE_H_

#include "LockedQueue.h"
#include <functional>

#include <memory>

#include "Move.h"

enum class MessageID {
	Post,
	Cancel,
	
	Timeout,     // タイム後
	Initialized, // 初期化完了
	Ready,       // Ready
	BestMove,    // BestMove
	Stop,        // 停止
	Error,       // エラー
};

class Message {
	MessageID id_;
public:
	Message(MessageID id) { this->id_ = id; }
	virtual ~Message() {}

	MessageID ID() const { return this->id_; }
};


class MessagePost : public Message
{
	std::function<void()> func_ = nullptr;

public:
	MessagePost(std::function<void()> func) : Message(MessageID::Post)
	{
		this->func_ = func;
	}

	virtual ~MessagePost() {}

	void Func()
	{
		if (this->func_ != nullptr)
		{
			this->func_();
		}
	}
};

class MessageInitialized : public Message {
public:
	MessageInitialized(Color color) : Message(MessageID::Initialized)
	{
		this->Player = color;
	}

	Color Player;
};

class MessageReady : public Message {
public:
	MessageReady(Color color) : Message(MessageID::Ready)
	{
		this->Player = color;
	}

	Color Player;
};


class MessageBestMove : public Message {
public:
	MessageBestMove(Color color, int transactionNo, const Move& move, const Move& ponder) : Message(MessageID::BestMove)
	{
		this->Player = color;
		this->BestMove = move;
		this->Ponder = ponder;
		this->TransactionNo = transactionNo;
	}

	Color Player;
	Move BestMove;
	Move Ponder;
	int TransactionNo;
};

class MessageStop : public Message {
public:
	MessageStop(Color color, int transactionNo) : Message(MessageID::Stop)
	{
		this->Player = color;
		this->TransactionNo = transactionNo;
	}

	Color Player;
	int TransactionNo;
};

class MessageError : public Message {
public:
	MessageError(Color color, int error) : Message(MessageID::Error)
	{
		this->Error = error;
		this->Player = color;
	}

	Color Player;
	int Error;

};



class MessageQueue : public LockedQueue<std::shared_ptr<Message>>
{

public:
	MessageQueue() : LockedQueue(100) { }
	~MessageQueue() {}


	void Send(std::shared_ptr<Message> msg)
	{
		this->enqueue(msg);
	}

	std::shared_ptr<Message> Receive()
	{
		return this->dequeue();
	}

};




#endif

