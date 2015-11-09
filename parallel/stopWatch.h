#pragma once

class stopWatch
{
	LARGE_INTEGER mBegin;
	LARGE_INTEGER mFrequency;
public:
	stopWatch();
	double seconds();
};

