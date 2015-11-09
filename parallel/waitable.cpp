#include "stdafx.h"
#include "waitable.h"
#include <algorithm>


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

void waitable::waitForAll(size_t count, waitable ** waitables)
{
	HANDLE handles[MAXIMUM_WAIT_OBJECTS];
	for (int i = 0; i < count;) {
		int p = std::min(count - i, (unsigned long long) MAXIMUM_WAIT_OBJECTS);
		for (int j = 0; j < p; ++j) {
			handles[j] = waitables[i + j]->mHandle;
		}
		WaitForMultipleObjects(p, handles, true, INFINITE);
		i += p;
	}
}