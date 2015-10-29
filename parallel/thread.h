#pragma once

#include "waitable.h"

class thread : public waitable
{
public:
	void start(LPTHREAD_START_ROUTINE function, LPVOID parameter);
	virtual ~thread() override;
};

