#include "stdafx.h"
#include "semaphore.h"

semaphore::semaphore(unsigned initial, unsigned maximum)
{
	mHandle = CreateSemaphore(nullptr, initial, maximum, nullptr);
}

semaphore::semaphore(unsigned initial) : semaphore(initial, initial) {};

void semaphore::signal()
{
	ReleaseSemaphore(mHandle, 1, nullptr);
}
