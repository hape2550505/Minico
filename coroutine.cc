#include "coroutine.h"
#include "schedule.h"
#include "parameter.h"

using namespace minico;

static void coWrapFunc(Schedule* sche)
{
	sche->getCurRunningCo()->startFunc();//����ʱ�Ҹ���ʲô����ôִ�к���
	sche->killCurCo();//��Э���������ɾ��������� ����ɾ��pCurCoroutine_���ÿ�
	sche->resumeAnotherCoroutine();//Э���˳�������Զ�ת�Ƶ����Э��
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
	case READY://Э�̵ĵ�һ��ִ��
        status_ = RUNNING;
		ctx_.makeContext((void (*)(void)) coWrapFunc, pMySchedule_);//��¼��ǰ�����������
		if (isFirstCo)//�����ǰֻ����һ��Э��
		{
			ctx_.swapToMe(pMySchedule_->getMainCtx());//���������̵������� ��Э��ִ������Զ��ظ�ȥִ��������
		}
		else
		{
			ctx_.swapToMe(pLastCo ? pLastCo->getCtx() : nullptr);//�õ���һ�����̵������� Ϊ�վͼ���ִ�����Э��
		}
		break;

	case WAITING:
		status_ = RUNNING;
		ctx_.swapToMe(pLastCo ? pLastCo->getCtx() : nullptr);
		break;

	default://runing̬����ִ��

		break;
	}
}

void Coroutine::yield()
{
	status_ = WAITING;
}
select * from employees
where hire_date = (select max(hire_date) from employees);