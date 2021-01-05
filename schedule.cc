#include "schedule.h"

using namespace minico;

Schedule* Schedule::scheduler_ = nullptr;
std::mutex Schedule::initMutex_;

Schedule::Schedule()
        : isFirstGo_(true), pLastCoroutine_(nullptr), pCurCoroutine_(nullptr)
{
	mainCtx_.makeCurContext();//主进程的进入与退出不需要设置参数 且不需要退出后切换到别的函数
}

Schedule::~Schedule()
{ }

Schedule* Schedule::getSchedule()
{
	if (!scheduler_)
	{
		std::lock_guard<std::mutex> lock(initMutex_);
		if (!scheduler_)
		{
			scheduler_ = new Schedule();
		}
	}
	return scheduler_;
}

void Schedule::resume(Coroutine* pCo)
{
	if (nullptr == pCo)
	{
		return;
	}
	pLastCoroutine_ = pCurCoroutine_;
	if (pLastCoroutine_)//如果有将现在正在执行的协程设为等待插入队列
	{
	    pLastCoroutine_->yield();//简单设置状态为等待 保存上下文是在pCo->resume();进行的
		coroutines_.push(pLastCoroutine_);
	}
	pCurCoroutine_ = pCo;
	if (isFirstGo_)//根据是否为唯一的协程控制切换上下文
	{
		isFirstGo_ = false;
		pCo->resume(true);
	}
	else
	{
		pCo->resume();
	}
}

void Schedule::NewCo(std::function<void()>&& coFunc)
{
	Coroutine* pCo = new Coroutine(this, std::move(coFunc));
	resume(pCo);
}

void Schedule::NewCo(std::function<void()>& coFunc)
{
	Coroutine* pCo = new Coroutine(this, coFunc);
	resume(pCo);
}

void Schedule::killCurCo()
{
	delete pCurCoroutine_;
	pCurCoroutine_ = nullptr;
}

void Schedule::resumeAnotherCoroutine()
{
	if (coroutines_.empty())
	{
		isFirstGo_ = true;
		mainCtx_.swapToMe(nullptr);//此时继续执行主进程去了下面这些东西不会再执行了
	}
	Coroutine* pCo = coroutines_.front();//取出队头并执行
	coroutines_.pop();
	resume(pCo);
}
