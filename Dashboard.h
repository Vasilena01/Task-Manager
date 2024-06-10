#pragma once
#include <iostream>
#include "Task.h"
#include "Vector.hpp"

class Dashboard
{
public:
	void addTask(const Task& task);
	void removeTask(unsigned id);
	void listTasks() const;
private:
	Vector<Task> tasks;
};

