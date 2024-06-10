#include "Dashboard.h"

void Dashboard::addTask(const Task& task)
{
	tasks.pushBack(task);
}

void Dashboard::removeTask(unsigned id)
{
    for (int i = 0; i < tasks.getSize(); i++)
    {
        if (tasks[i].getId() == id)
            tasks.popAt(i);
    }
}

void Dashboard::listTasks() const
{
    for (int i = 0; i < tasks.getSize(); i++)
        tasks[i].printTask();
}
