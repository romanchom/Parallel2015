#pragma once
class semaphore
{
public:
	semaphore(unsigned initial);
	~semaphore();
	void wait();
	void signal();
private:
	HANDLE mHandle;
};

