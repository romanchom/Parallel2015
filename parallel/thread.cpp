#include "stdafx.h"
#include "thread.h"

void thread::start(LPTHREAD_START_ROUTINE function, LPVOID parameter)
{
	mHandle = CreateThread(nullptr, 0, function, parameter, 0, nullptr);
}

thread::~thread()
{
	DWORD result = WaitForSingleObject(mHandle, 0);
	if (result != WAIT_OBJECT_0) { // thread still alive, lets kill it the ugly way
		TerminateProcess(mHandle, -1);
	}
}