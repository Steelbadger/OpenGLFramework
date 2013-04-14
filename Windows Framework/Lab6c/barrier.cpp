#include "Barrier.h"


Barrier::Barrier()
{
	semaphore = CreateSemaphore(NULL, 0, 64, NULL);
	waiting = 0;
	target = 0;
	InitializeCriticalSection(&waitingMutex);
	InitializeCriticalSection(&targetMutex);
	barrierActive = false;
}

void Barrier::NewThread()
{
	EnterCriticalSection(&targetMutex);
	target++;
	barrierActive = true;
	LeaveCriticalSection(&targetMutex);
}

void Barrier::Wait()
{
	EnterCriticalSection(&waitingMutex);
	waiting++;

	if(waiting >= target)
	{
		waiting = 0;
		LeaveCriticalSection(&waitingMutex);
		ReleaseSemaphore(semaphore, target-1, NULL);
		target = 0;
		barrierActive = false;
	}
	else
	{
		LeaveCriticalSection(&waitingMutex);
		WaitForSingleObject(semaphore, INFINITE);
	}
}

bool Barrier::Check()
{
	return barrierActive;
}

Barrier::~Barrier(void)
{
}
