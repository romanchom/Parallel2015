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

void waitable::waitForAll(size_t count, waitable waitables[])
{
	HANDLE * handles = new HANDLE[count];
	for (int i = 0; i < count; ++i) {
		handles[i] = waitables[i].mHandle;
	}
	DWORD ret = WaitForMultipleObjects(count, handles, true, INFINITE);
	delete[] handles;
}

void waitable::waitForAny(size_t count, waitable waitables[])
{
	HANDLE * handles = new HANDLE[count];
	for (int i = 0; i < count; ++i) {
		handles[i] = waitables[i].mHandle;
	}
	WaitForMultipleObjects(count, handles, false, INFINITE);
	delete[] handles;
}