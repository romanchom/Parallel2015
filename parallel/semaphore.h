#pragma once

#include "waitable.h"

class semaphore : public waitable
{
public:
	semaphore(unsigned initial);
	void signal();
};

