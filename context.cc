#include "context.h"
#include "parameter.h"
#include <stdlib.h>

using namespace minico;
Context::Context()
	:pCtx_(nullptr),pStack_(nullptr)
{ }

Context::~Context()
{
	if (pCtx_)
	{
		delete pCtx_;
	}
	if (pStack_)
	{
		free(pStack_);
	}
}

void Context::makeContext(void (*func)(), Schedule* pSche)
{
	if (nullptr == pCtx_)
	{
		pCtx_ = new  ucontext_t;
	}
	if (nullptr == pStack_)
	{
		pStack_ = malloc(coroutineStackSize);//分配固定大小空间保存上下文
	}
	::getcontext(pCtx_);
    pCtx_->uc_stack.ss_sp = pStack_;//上下文入栈
    pCtx_->uc_stack.ss_size = parameter::coroutineStackSize;
    pCtx_->uc_link = NULL;
	makecontext(pCtx_, func, 1, pSche);
}

void Context::makeCurContext()
{
	if (nullptr == pCtx_)
	{
		pCtx_ = new ucontext_t;
	}
    ::getcontext(pCtx_);
}

void Context::swapToMe(Context* pOldCtx)
{
	if (nullptr == pOldCtx)
	{
		setcontext(pCtx_);//继续执行
	}
	else
	{
		swapcontext(pOldCtx->getUCtx(),pCtx_);//执行pCtx_执行完执行pOldCtx
	}
}