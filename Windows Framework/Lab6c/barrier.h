#pragma once

#include <Windows.h>

class Barrier
{
public:
	Barrier();
	~Barrier(void);

	void NewThread();
	void Wait();
	bool Check();

private:
	int waiting;
	int target;

	bool barrierActive;

	CRITICAL_SECTION waitingMutex;
	CRITICAL_SECTION targetMutex;

	HANDLE semaphore;
};

