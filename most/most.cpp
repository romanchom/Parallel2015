// most.cpp : Defines the entry point for the console application.
//

#include "monitor.h"
#include "monitorLock.h"
#include "thread.h"
#include <iostream>
#include <random>

monitor<int> bridge;

unsigned long WINAPI carThread(void * data) {
	int parity = *(int *)data;

	monitorLock lock(bridge);

	while ((bridge.getValue() / 10 ^ parity) & 1) bridge.wait();

	bridge.setValue(bridge.getValue() + 1);
	std::cout << "Car " << parity << " crossing the river\n";

	return 0;
}

int main()
{
	const int carCount = 60;
	thread * cars = new thread[carCount];
	int * side = new int[carCount];

	std::default_random_engine rand(123123);
	std::uniform_int_distribution<int> dist(0, 1000);

	for (int i = 0; i < carCount; ++i) {
		//side[i] = dist(rand);
		side[i] = i;
		cars[i].start(carThread, side + i);
	}


	waitable::waitForAll(carCount, cars);
	delete[] side;
	delete[] cars;
    return 0;
}

