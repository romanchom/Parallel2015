#include "stdafx.h"
#include "monitorLock.h"

#include "monitor.h"

monitorLock::monitorLock(monitorBase & monit) :
	mCriticalSection(&monit.mCriticalSection)
{
	EnterCriticalSection(mCriticalSection);
}


monitorLock::~monitorLock()
{
	LeaveCriticalSection(mCriticalSection);
}
