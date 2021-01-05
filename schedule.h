#pragma once
#include <queue>
#include <mutex>
#include "context.h"

#include "coroutine.h"

//����һ����Э��
#define co_start(func) minico::Schedule::getSchedule()->NewCo(func)

//��ͣ��ǰЭ��
#define co_yield() minico::Schedule::getSchedule()->resumeAnotherCoroutine()

namespace minico
{

	class Schedule
	{
	public:
		Schedule(const Schedule&) = delete;
		Schedule(const Schedule&&) = delete;  
		Schedule& operator=(const Schedule&) = delete;
		//��ȡ��ǰ����������
		static Schedule* getSchedule();

		//����һ����Э�̣���Э�̵ĺ�����func
		void NewCo(std::function<void()>&& func);//����������ת����
		void NewCo(std::function<void()>& func);

		//�ָ�������һ��Э��
		void resumeAnotherCoroutine();

		//�����ǰ�������е�Э��
		void killCurCo();

		//��ȡ��һ�����е�Э��
		Coroutine* getLastCoroutine() { return pLastCoroutine_; };

		//��ȡ��ǰ�������е�Э��
		Coroutine* getCurRunningCo() { return pCurCoroutine_; };

		Context* getMainCtx() { return &mainCtx_; }

	private:
		Schedule();
		~Schedule();

		//�ָ�����ָ��Э��
		void resume(Coroutine*);

		static Schedule* scheduler_;

		static std::mutex initMutex_;

		bool isFirstGo_;

		//FIFO queue
		std::queue<Coroutine*> coroutines_;

		Coroutine* pLastCoroutine_;//ָ�����ָ��

		Coroutine* pCurCoroutine_;

		Context mainCtx_;

	};

}