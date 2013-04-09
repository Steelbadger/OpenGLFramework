#pragma once

#include <windows.h>
#include <process.h>



class BaseTask
{
public:
	BaseTask(void);
	~BaseTask(void);
	void Completed();

	virtual void Execute() = 0;

private:
	HANDLE deathSem;
};