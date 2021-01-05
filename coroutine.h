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


		//�ָ����е�ǰЭ��
		void resume(bool isFirstCo = false);

		//��ͣ���е�ǰЭ��
		void yield();

		//���и�Э�̵ĺ���
		void startFunc() { coFunc_(); };

		//��ȡ��Э�̵�������
		Context* getCtx() { return &ctx_; }

	private:

		std::function<void()> coFunc_;

		Schedule* pMySchedule_;

		int status_;

		Context ctx_;

	};

}