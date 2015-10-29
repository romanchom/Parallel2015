#include "stdafx.h"
#include "semaphore.h"

semaphore::semaphore(unsigned initial)
{
	mHandle = CreateSemaphore(nullptr, initial, initial, L"Semaphore");
}

semaphore::~semaphore()
{
	CloseHandle(mHandle);
}

void semaphore::wait()
{
	WaitForSingleObject(mHandle, -1);
}

void semaphore::signal()
{
	ReleaseSemaphore(mHandle, 1, nullptr);
}
