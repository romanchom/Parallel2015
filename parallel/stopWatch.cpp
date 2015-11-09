#include "stdafx.h"
#include "stopWatch.h"


stopWatch::stopWatch()
{
	QueryPerformanceCounter(&mBegin);
	QueryPerformanceFrequency(&mFrequency);
}

double stopWatch::seconds() {
	LARGE_INTEGER now;
	QueryPerformanceCounter(&now);
	UINT64 diff = now.QuadPart - mBegin.QuadPart;
	return diff / (double) mFrequency.QuadPart;
}
