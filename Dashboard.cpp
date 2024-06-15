#include "Dashboard.h"

//void Dashboard::addTask(const Task& task)
//{
//    tasks.addTask(task);
//	//tasks.pushBack(task);
//}

bool Dashboard::isTaskInDashboard(unsigned id)
{
    for (int i = 0; i < tasks.getSize(); i++)
    {
        if (tasks[i] != nullptr && tasks[i]->getId() == id)
            return true;
    }
    return false;
}

void Dashboard::addTask(SharedPtr<Task> task)
{
    for (int i = 0; i < tasks.getSize(); i++)
    {
        if (tasks[i] != nullptr && tasks[i]->getName() == task->getName() && tasks[i]->getDate() == task->getDate()
            && tasks[i]->getDescription() == task->getDescription())
            throw std::logic_error("This task is already added to the dashboard!");
    }
    tasks.addTask(task);
}

void Dashboard::removeTask(unsigned id)
{
    for (int i = 0; i < tasks.getSize(); i++)
    {
        if (tasks[i] != nullptr && tasks[i]->getId() == id)
        {
            tasks[i] = nullptr;
            return;
            //tasks.popAt(i);
        }
    }
    throw std::logic_error("Task not found in the dashboard!");
}

void Dashboard::listTasks() const
{
    for (int i = 0; i < tasks.getSize(); i++)
    {
        if (tasks[i] != nullptr)
            tasks[i]->printTask();
    } 
}
