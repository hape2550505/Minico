#pragma once
#include <functional>
#include "context.h"

namespace minico
{

	enum coStatus
	{
		READY = 0,
		RUNNING,
		WAITING,
		DEAD
	};

	class Schedule;

	class Coroutine
	{
	public:
		Coroutine(Schedule*, std::function<void()>&&);
		Coroutine(Schedule*, std::function<void()>&);
		~Coroutine();

		Coroutine(const Coroutine&) = delete; 
		Coroutine(const Coroutine&&) = delete; 
		Coroutine& operator=(const Coroutine&) = delete;


		//恢复运行当前协程
		void resume(bool isFirstCo = false);

		//暂停运行当前协程
		void yield();

		//运行该协程的函数
		void startFunc() { coFunc_(); };

		//获取该协程的上下文
		Context* getCtx() { return &ctx_; }

	private:

		std::function<void()> coFunc_;

		Schedule* pMySchedule_;

		int status_;

		Context ctx_;

	};

}