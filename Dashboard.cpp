#include "Dashboard.h"

bool Dashboard::isTaskInDashboard(unsigned id)
{
    for (int i = 0; i < tasks.getSize(); i++)
    {
        if (tasks[i] != nullptr && tasks[i]->getId() == id)
            return true;
    }
    return false;
}

void Dashboard::addTask(Task* task)
{
    for (int i = 0; i < tasks.getSize(); i++)
    {
        if (tasks[i] != nullptr && tasks[i]->getName() == task->getName() 
            && GlobalFunctions::compareDates(tasks[i]->getDate(), task->getDate().getValue())
            && tasks[i]->getDescription() == task->getDescription())
            throw std::logic_error("This task is already added to the dashboard!");
    }
    tasks.addTask(task);
}

void Dashboard::removeTask(unsigned id)
{
    tasks.removeTask(id);
    //for (int i = 0; i < tasks.getSize(); i++)
    //{
    //    if (tasks[i] != nullptr && tasks[i]->getId() == id)
    //    {
    //        delete tasks[i];
    //        // TODO da se opravi, uj tasks[i] bilo const...
    //        //tasks[i] = nullptr;
    //        return;
    //    }
    //}
    //throw std::logic_error("Task not found in the dashboard!");
}

void Dashboard::listTasks()
{
    for (int i = 0; i < tasks.getSize(); i++)
    {
        if (tasks[i] != nullptr)
            tasks[i]->printTask();
    } 
}

TasksCollection Dashboard::getTasks() const
{
    return tasks;
}