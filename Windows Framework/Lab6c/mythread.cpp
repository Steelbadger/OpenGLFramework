#include "mythread.h"


MyThread::MyThread(void):
	threadActive(true),
	taskComplete(false),
	taskAssigned(false),
	inProcess(false),
	doOnce(false)
{
	handle = (HANDLE) _beginthreadex(NULL,0,&MyThread::ThreadFunc,(void*)this,CREATE_SUSPENDED,NULL);
	taskSem = CreateSemaphore(NULL, 0, 1, NULL);
	deathSem = CreateSemaphore(NULL, 0, 1, NULL);
	threadCompleteSem = CreateSemaphore(NULL, 0, 1, NULL);
	taskCompleteSem = CreateSemaphore(NULL, 0, 1, NULL);
}

MyThread::MyThread(BaseTask& t):
	threadActive(true),
	taskComplete(false),
	taskAssigned(false),
	inProcess(false),
	doOnce(false)
{
	AssignTask(t);
	handle = (HANDLE) _beginthreadex(NULL,0,&MyThread::ThreadFunc,(void*)this,CREATE_SUSPENDED,NULL);
	taskSem = CreateSemaphore(NULL, 0, 1, NULL);
	deathSem = CreateSemaphore(NULL, 0, 1, NULL);
	threadCompleteSem = CreateSemaphore(NULL, 0, 1, NULL);
	taskCompleteSem = CreateSemaphore(NULL, 0, 1, NULL);
}



MyThread::~MyThread(void)
{
	ReleaseSemaphore(deathSem, 1, NULL);
	WaitForSingleObject(threadCompleteSem, INFINITE);
}

void MyThread::AssignTask(BaseTask& task)
{
	currentTask = &task;
	taskComplete = false;
	taskAssigned = true;
	inProcess = false;
}

void MyThread::BeginTask()
{
	if (taskAssigned == true && inProcess == false) {
		taskComplete = false;
		inProcess = true;
		ResumeThread(handle);
		ReleaseSemaphore(taskSem, 1, NULL);
	}
}

bool MyThread::TaskComplete()
{
	return taskComplete;
}

void MyThread::WaitForTask()
{
	WaitForSingleObject(taskCompleteSem, INFINITE);
	ReleaseSemaphore(taskCompleteSem, 1, NULL);
}

unsigned __stdcall MyThread::ThreadFunc(void* args)
{
	//  Retrieve the thread-object from the argument
	MyThread* p = (MyThread*) args;

	HANDLE deathSem = p->deathSem;
	DWORD deathCheckResult;

	while(p->threadActive) {
		WaitForSingleObject(p->taskSem, INFINITE);
		WaitForSingleObject(p->taskCompleteSem, 0L);
		BaseTask* currentTask = p->currentTask;
		bool taskComplete = p->taskComplete;
		bool inProcess = p->inProcess;

		//  Execute the current task for that thread object
		p->currentTask->Execute();

		p->taskComplete = true;
		p->inProcess = false;

		deathCheckResult = WaitForSingleObject(p->deathSem, 0L);
		if (deathCheckResult == WAIT_TIMEOUT) {
			p->threadActive = false;
			p->currentTask->Completed();
		}
		ReleaseSemaphore(p->taskCompleteSem, 1, NULL);
	}
	ReleaseSemaphore(p->threadCompleteSem, 1, NULL);
	//  Once done return 0
	return 0;
}
