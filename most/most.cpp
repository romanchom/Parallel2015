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
	const int carCount = 40;
	thread ** cars = new thread*[carCount];
	int * side = new int[carCount];

	for (int i = 0; i < carCount; ++i) {
		side[i] = i;
		cars[i] = new thread();
		cars[i]->start(carThread, side + i);
	}


	waitable::waitForAll(carCount, (waitable**) cars);

	for (int i = 0; i < carCount; ++i) {
		delete cars[i];
	}
	delete[] side;
	delete[] cars;
    return 0;
}

