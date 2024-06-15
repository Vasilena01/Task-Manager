#pragma once
#include <iostream>
#include "SharedPtr.hpp"
#include "Task.h"
//#include "Vector.hpp"
#include "TasksCollection.h"

class Dashboard
{
public:
	bool isTaskInDashboard(unsigned id);
	void addTask(SharedPtr<Task> task);
	void removeTask(unsigned id);
	void listTasks() const;
private:
	//Vector<Task> tasks;
	TasksCollection tasks;
};

