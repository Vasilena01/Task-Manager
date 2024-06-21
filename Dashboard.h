#pragma once
#include <iostream>
#include "GlobalFunctions.h"
#include "Task.h"
#include "TasksCollection.h"

class Dashboard
{
public:
	Dashboard() = default;

	bool isTaskInDashboard(unsigned id);
	void addTask(Task* task);
	void removeTask(unsigned id);
	void listTasks();
	TasksCollection getTasks() const;
private:
	TasksCollection tasks;
};

