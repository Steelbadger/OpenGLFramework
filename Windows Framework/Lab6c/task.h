#pragma once



class BaseTask
{
public:
	BaseTask(void){};
	~BaseTask(void);

	virtual void Execute() = 0;
};

template <class returnType, class argType>
class Task : public BaseTask
{
public:
	Task():assigned(false){};
	Task(returnType (*function)(argType), argType args):functionPointer(function), arguments(args), assigned(true){};
	~Task(){};

	void Execute(){
		*functionPointer(arguments);
	}

private:
	returnType (*functionPointer)(argType);
	argType arguments;
	returnType returnValue;
	bool assigned;
};

