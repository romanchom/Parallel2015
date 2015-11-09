#include "stdafx.h"

#include <iostream>
#include <fstream>
#include "semaphore.h"
#include "thread.h"
#include "threadTask.h"
#include "stopWatch.h"

struct finalResult {
	size_t result[256];
	semaphore sem;

	finalResult() : sem(1) { memset(result, 0, sizeof(result)); }

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

	task.pFinalResult->sem.wait();
	for (int i = 0; i < 256; ++i) {
		task.pFinalResult->result[i] += result[i];
	}
	task.pFinalResult->sem.signal();

	return 0;
}

struct workItem {
	threadTask task;
	thread * t;
};


int main(int argc, char * argv[])
{
	if (argc < 3) {
		std::cout << "Parametry: liczba watkow, nazwa pliku" << std::endl;
		return -1;
	}
	int threadCount = atoi(argv[1]);

	size_t dataLenght;
	unsigned char * data;
	{
		std::ifstream file(argv[2], std::fstream::in | std::fstream::binary);
		if (!file.good()) return -2;
		file.seekg(0, std::ios::end);
		dataLenght = file.tellg();
		file.seekg(0, std::ios::beg);

		data = new unsigned char[dataLenght];
		file.read((char *) data, dataLenght);

	}

	finalResult result;

	threadTask * tasks = new threadTask[threadCount];
	thread ** threads = new thread*[threadCount];
	unsigned char * begin = data;

	for (int i = 0; i < threadCount; ++i) {
		threadTask & task = tasks[i];
		task.begin = begin;
		begin = (unsigned char *)data + (dataLenght * (i + 1) / threadCount);
		task.end = begin;
		task.pFinalResult = &result;
		threads[i] = new thread();
	}
	stopWatch time;
	for (int i = 0; i < threadCount; ++i) {
		threads[i]->start(threadFunc, (void *) &tasks[i]);
	}
	
	waitable::waitForAll(threadCount, (waitable **) threads); // kinda ugly hack
	double s = time.seconds();
	std::cout << "Time: " << s << std::endl;
	//result.printResult();

	delete[] data;

	for (int i = 0; i < threadCount; ++i) {
		delete threads[i];
	}
	delete[] threads;
	delete[] tasks;

	std::cin.get();
	return 0;
}

