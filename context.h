#pragma once
#include "utils.h"
#include <ucontext.h>
const static size_t coroutineStackSize = 32 * 1024;
namespace minico
{

    class Schedule;
	class Context
	{
	public:
		Context();
		~Context();

		Context(const Context& otherCtx) 
			: pCtx_(otherCtx.pCtx_), pStack_(otherCtx.pStack_)
		{ }

		Context(Context&& otherCtx)
			: pCtx_(otherCtx.pCtx_), pStack_(otherCtx.pStack_)
		{ }

		Context& operator=(const Context& otherCtx) = delete;

		//�ú���ָ�����õ�ǰcontext�����������
		void makeContext(void (*func)(), Schedule*);

		//ֱ���õ�ǰ����״̬���õ�ǰcontext��������
		void makeCurContext();

		//����ǰ�����ı��浽oldCtx�У�Ȼ���л�����ǰ�����ģ���oldCtxΪ�գ���ֱ������
		void swapToMe(Context* pOldCtx);

		//��ȡ��ǰ�����ĵ�ucontext_tָ��
		ucontext_t* getUCtx() { return pCtx_; };

	private:

		ucontext_t* pCtx_;

		void* pStack_;

	};

}