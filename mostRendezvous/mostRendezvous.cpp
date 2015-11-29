#include "monitor.h"
#include "monitorLock.h"
#include "thread.h"
#include "semaphore.h"
#include <iostream>
#include <random>
#include <queue>

struct carData
{
	int parity;
	bool crossed;
	semaphore lock;

	carData() : parity(0), crossed(false), lock(1) {}
	carData(int p) : parity(p), crossed(false), lock(1) {}
};

volatile bool abortServer;
std::queue<carData*> taskQueue;
semaphore taskQueueLock(1);

void crossBridgeRandezvous(carData* data)
{
	taskQueueLock.wait();
	taskQueue.push(data);
	taskQueueLock.signal();

	bool crossed = false;
	while (!crossed)
	{
		data->lock.wait();
		crossed = data->crossed;
		data->lock.signal();
	}
}

unsigned long WINAPI carThread(void* data) 
{
	carData* myData = (carData*)data;

	crossBridgeRandezvous(myData);

	return 0;
}

unsigned long WINAPI serverThread(void* data)
{
	uint32_t bridgeCounter = 0;
	carData* car = nullptr;

	while (!abortServer)
	{
		taskQueueLock.wait();
		if (!taskQueue.empty())
		{
			car = taskQueue.front();
			taskQueue.pop();
			if (!((bridgeCounter / 10 ^ car->parity) & 1))
			{
				car->lock.wait();
				car->crossed = true;
				car->lock.signal();
				std::cout << "Car " << car->parity << " crossing the river.\n";
				bridgeCounter++;
			}
			else
			{
				taskQueue.push(car);
			}
		}
		taskQueueLock.signal();
	}

	return 0;
}

int main()
{
	const int carCount = 40;
	thread** cars = new thread*[carCount];
	carData* carDatas = new carData[carCount];

	abortServer = false;
	thread* server = new thread();
	server->start(serverThread, nullptr);

	for (int i = 0; i < carCount; ++i) 
	{
		carDatas[i].parity = i;
		cars[i] = new thread();
		cars[i]->start(carThread, carDatas + i);
	}

	waitable::waitForAll(carCount, (waitable**)cars);
	abortServer = true;
	server->wait();

	for (int i = 0; i < carCount; ++i) 
	{
		delete cars[i];
	}
	delete[] carDatas;
	delete[] cars;
	delete server;

	return 0;
}

