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
	WaitForMultipleObjects(count, handles, true, -1);
	delete[] handles;
}

void waitable::waitForAny(size_t count, waitable waitables[])
{
	HANDLE * handles = new HANDLE[count];
	for (int i = 0; i < count; ++i) {
		handles[i] = waitables[i].mHandle;
	}
	WaitForMultipleObjects(count, handles, false, -1);
	delete[] handles;
}