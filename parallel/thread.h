#pragma once
class thread
{
public:
	thread(unsigned long (WINAPI* function)(void *), void * parameter);
	~thread();
	void join();
private:
	HANDLE mHandle;
};

