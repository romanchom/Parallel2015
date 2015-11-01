#include "stdafx.h"
#include "waitable.h"


waitable::waitable()
{
}


waitable::~waitable()
{
	CloseHandle(mHandle);
}

void waitable::wait()
{
	WaitForSingleObject(mHandle, -1);
}


void waitable::doWait(size_t count, waitable waitables[], bool all)
{
	HANDLE handles[MAXIMUM_WAIT_OBJECTS];
	for (int i = 0; i < count;) {
		int p = count - i;
		for (int j = 0; j < p; ++j) {
			handles[j] = waitables[i].mHandle;
		}
		WaitForMultipleObjects(p, handles, all, INFINITE);
		i += p;
	}
}

void waitable::waitForAll(size_t count, waitable waitables[])
{
	doWait(count, waitables, true);
}

void waitable::waitForAny(size_t count, waitable waitables[])
{
	doWait(count, waitables, false);
}