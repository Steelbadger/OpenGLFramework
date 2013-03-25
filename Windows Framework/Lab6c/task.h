#pragma once



class BaseTask
{
public:
	BaseTask(void):assigned(false){};
	~BaseTask(void);
	virtual void SetFunction() = 0;

	virtual void Execute() = 0;

protected:
	bool assigned;
};

template <class returnType, class argType>
class Task : public BaseTask
{
public:
	Task():BaseTask(){};
	Task(returnType (*function)(void*), argType args):functionPointer(function), arguments(args), assigned(true){};
	~Task(){};

	virtual void SetFunction(returnType (*function)(argType), argType args){
		functionPointer = function;
		arguments = args;
		assigned = true;
	}
	virtual void Execute(){
		returnvalue = *functionPointer(arguments);
	}

private:
	returnType (*functionPointer)(argType);
	argType arguments;
	returnType returnValue;
};

