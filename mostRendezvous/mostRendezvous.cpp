#include "monitor.h"
#include "monitorLock.h"
#include "thread.h"
#include "semaphore.h"
#include <iostream>
#include <random>
#include <queue>

struct carData
{
	uint32_t parity;
	semaphore crossed;

	carData() : parity(0), crossed(0, 1) {}
	carData(uint32_t p) : parity(p), crossed(0, 1) {}
};

volatile bool abortServer;
std::queue<carData*> taskQueue;
semaphore taskQueueLock(1);
HANDLE timer;

void crossBridgeRendezvous(carData* data)
{
	taskQueueLock.wait();
	taskQueue.push(data);
	taskQueueLock.signal();

	data->crossed.wait();
	//WaitForSingleObject(timer, 50);
}

unsigned long WINAPI carThread(void* data) 
{
	carData* myData = (carData*)data;

	crossBridgeRendezvous(myData);

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
				car->crossed.signal();
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
	timer = CreateEvent(nullptr, false, false, nullptr);

	const uint32_t carCount = 40;
	thread** cars = new thread*[carCount];
	carData* carDatas = new carData[carCount];

	abortServer = false;
	thread* server = new thread();
	server->start(serverThread, nullptr);

	for (uint32_t i = 0; i < carCount; ++i)
	{
		carDatas[i].parity = i;
		cars[i] = new thread();
		cars[i]->start(carThread, carDatas + i);
	}

	waitable::waitForAll(carCount, (waitable**)cars);
	abortServer = true;
	server->wait();

	for (uint32_t i = 0; i < carCount; ++i)
	{
		delete cars[i];
	}
	delete[] carDatas;
	delete[] cars;
	delete server;

	CloseHandle(timer);

	return 0;
}

