#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#include <Windows.h>

#include <iostream>
#include <fstream>
#include <vector>
#include "semaphore.h"
#include "thread.h"
#include "stopWatch.h"

const int numberOfSeats = 5;
const int numberOfCustomers = 15;

semaphore barberReady(0, 1);
semaphore seatsMutex(1);
semaphore customersWaiting(0, numberOfSeats);
volatile unsigned numberOfFreeSeats = numberOfSeats;

bool shopOpen = true;

unsigned long WINAPI barberFunc(void * data) {
	while (shopOpen) {
		customersWaiting.wait();
		seatsMutex.wait();
		numberOfFreeSeats++;
		barberReady.signal();
		seatsMutex.signal();

		std::cout << "Barber\tI am cutting someones hair" << std::endl;
		Sleep(20);
	}
	return 0;
}

unsigned long WINAPI customerFunc(void * data) {
	seatsMutex.wait();
	if (numberOfFreeSeats > 0) {
		numberOfFreeSeats--;
		std::cout << ((int) data) << "\tI have taken a seat." << std::endl;
		customersWaiting.signal();
		seatsMutex.signal();
		barberReady.wait();
		std::cout << ((int)data) << "\tI am having my hair cut. Yay." << std::endl;
	}
	else {
		seatsMutex.signal();
		std::cout << ((int)data) << "\tShop full. Not getting hair cut." << std::endl;
	}

	return 0;
}

semaphore sem(1);

int main()
{
	thread barber;
	barber.start(barberFunc, nullptr);
	thread * customers = new thread[numberOfCustomers];
	for (int i = 0; i < numberOfCustomers; ++i) {
		customers[i].start(customerFunc, (void*) i);
		Sleep(10);
	}
	for (int i = 0; i < numberOfCustomers; ++i) {
		customers[i].wait();
	}
	shopOpen = false;
	barber.wait();
	delete[] customers;
    return 0;
}

