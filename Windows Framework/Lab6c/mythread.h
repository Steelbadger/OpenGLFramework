#pragma once
#include "task.h"
#include <windows.h>
#include <process.h>

class MyThread
{
public:
	MyThread(void);
	MyThread(BaseTask& t);
	~MyThread(void);

	void AssignTask(BaseTask& t);
	void BeginTask();
	void WaitForTask();
	bool TaskComplete();

private:
	HANDLE handle;
	HANDLE taskSem;
	HANDLE deathSem;
	HANDLE threadCompleteSem;
	HANDLE taskCompleteSem;
	BaseTask* currentTask;
	bool taskAssigned;
	bool taskComplete;
	bool inProcess;
	bool threadActive;
	bool doOnce;
	static unsigned __stdcall ThreadFunc(void* args);
};

