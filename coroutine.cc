#include "coroutine.h"
#include "schedule.h"
#include "parameter.h"

using namespace minico;

static void coWrapFunc(Schedule* sche)
{
	sche->getCurRunningCo()->startFunc();//切入时我该做什么：怎么执行函数
	sche->killCurCo();//将协程运行完的删除操作打包 帮助删除pCurCoroutine_并置空
	sche->resumeAnotherCoroutine();//协程退出后可以自动转移到别的协程
}

Coroutine::Coroutine(Schedule* pMySchedule, std::function<void()>&& func)
	: coFunc_(std::move(func)), pMySchedule_(pMySchedule), status_(DEAD)//, ctx_(stack_, parameter::coroutineStackSize)
{
	status_ = READY;
}

Coroutine::Coroutine(Schedule* pMySchedule, std::function<void()>& func)
	: coFunc_(func), pMySchedule_(pMySchedule), status_(DEAD)//, ctx_(stack_, parameter::coroutineStackSize)
{
	status_ = READY;
}

Coroutine::~Coroutine()
{
}

void Coroutine::resume(bool isFirstCo)
{
	Coroutine* pLastCo = pMySchedule_->getLastCoroutine();
	switch (status_)
	{
	case READY://协程的第一次执行
        status_ = RUNNING;
		ctx_.makeContext((void (*)(void)) coWrapFunc, pMySchedule_);//记录当前程序的上下文
		if (isFirstCo)//如果当前只有这一个协程
		{
			ctx_.swapToMe(pMySchedule_->getMainCtx());//保存主进程的上下文 该协程执行完会自动回复去执行主进程
		}
		else
		{
			ctx_.swapToMe(pLastCo ? pLastCo->getCtx() : nullptr);//拿到上一个进程的上下文 为空就继续执行这个协程
		}
		break;

	case WAITING:
		status_ = RUNNING;
		ctx_.swapToMe(pLastCo ? pLastCo->getCtx() : nullptr);
		break;

	default://runing态继续执行

		break;
	}
}

void Coroutine::yield()
{
	status_ = WAITING;
}
select * from employees
where hire_date = (select max(hire_date) from employees);