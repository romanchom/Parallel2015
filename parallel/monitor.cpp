#include "stdafx.h"
#include "monitor.h"

monitorBase::monitorBase() :
	mConditionVariable({}),
	mCriticalSection({})
{
	InitializeConditionVariable(&mConditionVariable);
	InitializeCriticalSection(&mCriticalSection);
}


monitorBase::~monitorBase()
{
	DeleteCriticalSection(&mCriticalSection);
}

void monitorBase::wait()
{
	SleepConditionVariableCS(&mConditionVariable, &mCriticalSection, INFINITE);
}

void monitorBase::wakeConditionVariable()
{
	//WakeAllConditionVariable(&mConditionVariable);
	WakeConditionVariable(&mConditionVariable);
}
