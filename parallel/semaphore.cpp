#include "stdafx.h"
#include "semaphore.h"

semaphore::semaphore(unsigned initial)
{
	mHandle = CreateSemaphore(nullptr, initial, initial, L"Semaphore");
}

void semaphore::signal()
{
	ReleaseSemaphore(mHandle, 1, nullptr);
}
