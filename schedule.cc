#include "schedule.h"

using namespace minico;

Schedule* Schedule::scheduler_ = nullptr;
std::mutex Schedule::initMutex_;

Schedule::Schedule()
        : isFirstGo_(true), pLastCoroutine_(nullptr), pCurCoroutine_(nullptr)
{
	mainCtx_.makeCurContext();//�����̵Ľ������˳�����Ҫ���ò��� �Ҳ���Ҫ�˳����л�����ĺ���
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
	if (pLastCoroutine_)//����н���������ִ�е�Э����Ϊ�ȴ��������
	{
	    pLastCoroutine_->yield();//������״̬Ϊ�ȴ� ��������������pCo->resume();���е�
		coroutines_.push(pLastCoroutine_);
	}
	pCurCoroutine_ = pCo;
	if (isFirstGo_)//�����Ƿ�ΪΨһ��Э�̿����л�������
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
		mainCtx_.swapToMe(nullptr);//��ʱ����ִ��������ȥ��������Щ����������ִ����
	}
	Coroutine* pCo = coroutines_.front();//ȡ����ͷ��ִ��
	coroutines_.pop();
	resume(pCo);
}
