#include "stdafx.h"
#include "thread.h"

thread::thread(unsigned long (WINAPI *function)(void *), void * parameter)
{
	CreateThread(nullptr, 0, function, parameter, 0, nullptr);
}

thread::~thread()
{
	TerminateProcess(mHandle, -1);
	CloseHandle(mHandle);
}

void thread::join()
{
	WaitForSingleObject(mHandle, -1);
}
