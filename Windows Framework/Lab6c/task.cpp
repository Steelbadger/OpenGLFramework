#include "task.h"

#include <windows.h>
#include <process.h>


BaseTask::BaseTask()
{
	deathSem = CreateSemaphore(NULL, 0, 1, NULL);
}



BaseTask::~BaseTask()
{
	WaitForSingleObject(deathSem, INFINITE);
}

void BaseTask::Completed()
{
	ReleaseSemaphore(deathSem, 1, NULL);
}