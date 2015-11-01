#pragma once

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#include <Windows.h>

class monitorBase {
private:
	CRITICAL_SECTION mCriticalSection;
	CONDITION_VARIABLE mConditionVariable;
protected:
	void wakeConditionVariable();
public:
	monitorBase();
	~monitorBase();
	void wait();

	friend class monitorLock;
};

template<typename T>
class monitor : public monitorBase
{
private:
	T mVariable;
public:
	T getValue() { return mVariable; }
	void setValue(T val) { mVariable = val; wakeConditionVariable(); }
};

