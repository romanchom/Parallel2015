#pragma once
class monitorLock
{
public:
	monitorLock(class monitorBase & monit);
	~monitorLock();
private:
	CRITICAL_SECTION * mCriticalSection;
};

