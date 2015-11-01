#pragma once
class waitable
{
public:
	virtual ~waitable();
	void wait();

	static void waitForAll(size_t count, waitable waitables[]);
	template<typename... objects>
	static void waitForAll(waitable * w, objects... obj);
	
	static void waitForAny(size_t count, waitable waitables[]);
	template<typename... objects>
	static void waitForAny(waitable * w, objects... obj);


protected:
	waitable();
	HANDLE mHandle;
	static void doWait(size_t count, waitable waitables[], bool all);
private:
	static HANDLE getHandle(waitable * w) { return w->mHandle; }
};

template<typename ...objects>
void waitable::waitForAll(waitable * w, objects... obj)
{
	HANDLE handles[] = { getHandle(w), getHandle(obj)... };
	WaitForMultipleObjects(sizeof...(obj) + 1, handles, true, -1);
}

template<typename ...objects>
void waitable::waitForAny(waitable * w, objects... obj)
{
	HANDLE handles[] = { getHandle(w), getHandle(obj)... };
	WaitForMultipleObjects(sizeof...(obj)+1, handles, false, -1);
}

