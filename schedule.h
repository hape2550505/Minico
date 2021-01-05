#pragma once
#include <queue>
#include <mutex>
#include "context.h"

#include "coroutine.h"

//运行一个新协程
#define co_start(func) minico::Schedule::getSchedule()->NewCo(func)

//暂停当前协程
#define co_yield() minico::Schedule::getSchedule()->resumeAnotherCoroutine()

namespace minico
{

	class Schedule
	{
	public:
		Schedule(const Schedule&) = delete;
		Schedule(const Schedule&&) = delete;  
		Schedule& operator=(const Schedule&) = delete;
		//获取当前调度器单例
		static Schedule* getSchedule();

		//运行一个新协程，该协程的函数是func
		void NewCo(std::function<void()>&& func);//这里是完美转发吗
		void NewCo(std::function<void()>& func);

		//恢复运行另一个协程
		void resumeAnotherCoroutine();

		//清楚当前正在运行的协程
		void killCurCo();

		//获取上一个运行的协程
		Coroutine* getLastCoroutine() { return pLastCoroutine_; };

		//获取当前正在运行的协程
		Coroutine* getCurRunningCo() { return pCurCoroutine_; };

		Context* getMainCtx() { return &mainCtx_; }

	private:
		Schedule();
		~Schedule();

		//恢复运行指定协程
		void resume(Coroutine*);

		static Schedule* scheduler_;

		static std::mutex initMutex_;

		bool isFirstGo_;

		//FIFO queue
		std::queue<Coroutine*> coroutines_;

		Coroutine* pLastCoroutine_;//指向队列指针

		Coroutine* pCurCoroutine_;

		Context mainCtx_;

	};

}