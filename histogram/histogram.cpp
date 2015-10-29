#include "stdafx.h"

#include <iostream>
#include <fstream>
#include "semaphore.h"
#include "thread.h"
#include "threadTask.h"

struct finalResult {
	size_t result[256];
	semaphore mutex;

	finalResult() : mutex(1) { memset(result, 0, sizeof(result)); }

	void printResult() {
		for (int i = 0; i < 256; ++i) {
			std::cout << i << "\t:\t" <<result[i] << std::endl;
		}
	}
};

struct threadTask {
	unsigned char * begin;
	unsigned char * end;
	finalResult * pFinalResult;
};

unsigned long WINAPI threadFunc(void * param) {
	threadTask & task = *(threadTask *)param;
	size_t result[256];
	memset(result, 0, sizeof(result));
	for (unsigned char * p = task.begin; p != task.end; ++p) {
		result[*p]++;
	}

	task.pFinalResult->mutex.wait();
	for (int i = 0; i < 256; ++i) {
		task.pFinalResult->result[i] += result[i];
	}
	task.pFinalResult->mutex.signal();

	return 0;
}

struct workItem {
	threadTask task;
	thread * t;
};


int main(int argc, char * argv[])
{
	if (argc < 2) {
		std::cout << "Parametry: liczba watkow." << std::endl;
		return -1;
	}
	int threadCount = atoi(argv[1]);

	size_t dataLenght;
	unsigned char * data;
	{
		std::ifstream file(argv[0], std::fstream::in | std::fstream::binary);
		if (!file.good()) return -2;
		file.seekg(0, std::ios::end);
		dataLenght = file.tellg();
		file.seekg(0, std::ios::beg);

		data = new unsigned char[dataLenght];
		file.read((char *) data, dataLenght);

	}

	finalResult result;

	workItem * items = new workItem[threadCount];
	for (int i = 0; i < threadCount; ++i) {
		threadTask & task = items[i].task;
		task.begin	= (unsigned char *) data + (dataLenght * i / threadCount);
		task.end	= (unsigned char *) data + (dataLenght * (i + 1) / threadCount);
		task.pFinalResult = &result;

		items[i].t = new thread(threadFunc, (void *) &items[i].task);
	}

	for (int i = 0; i < threadCount; ++i) {
		items[i].t->join();
	}

	result.printResult();

	delete[] data;
	for (int i = 0; i < threadCount; ++i) {
		delete items[i].t;
	}
	delete[] items;

	std::cin.get();
	return 0;
}

