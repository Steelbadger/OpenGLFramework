#include "mythread.h"


MyThread::MyThread(void)
{
}

MyThread::MyThread(BaseTask* t)
{
	AssignTask(t);
}



MyThread::~MyThread(void)
{
}

void MyThread::AssignTask(BaseTask* task)
{
	currentTask = task;
	handle = (HANDLE) _beginthreadex(NULL,0,&MyThread::ThreadFunc,(void*)this,CREATE_SUSPENDED,NULL);
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
		inProcess = false;
		taskComplete = true;
	}
}

bool MyThread::TaskComplete()
{
	return taskComplete;
}

unsigned __stdcall MyThread::ThreadFunc(void* args)
{
	//  Retrieve the thread-object from the argument
	MyThread* p = (MyThread*) args;

	//  Execute the current task for that thread object
	p->currentTask->Execute();

	//  Once done return 0
	return 0;
}
