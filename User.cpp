#include "User.h"

Task& User::getTaskByID(unsigned id)
{
    for (int i = 0; i < tasks.getSize(); i++)
    {
        if (tasks[i].getId() == id)
            return tasks[i];
    }

    throw std::logic_error("There is no task with the given ID!");
}

const Task& User::getTaskByID(unsigned id) const
{
    for (int i = 0; i < tasks.getSize(); i++)
    {
        if (tasks[i].getId() == id)
            return tasks[i];
    }

    throw std::logic_error("There is no task with the given ID!");
}

Task& User::getTaskByName(const MyString& name)
{
    Task* taskToReturn = nullptr;

    for (int i = 0; i < tasks.getSize(); i++)
    {
        if (tasks[i].getName() == name)
        {
            if (taskToReturn == nullptr || tasks[i].getId() < taskToReturn->getId())
            {
                taskToReturn = &tasks[i];
            }
        }
    }

    if (taskToReturn == nullptr)
    {
        throw std::runtime_error("No task found with the given name!");
    }

    return *taskToReturn;
}

void User::addTask(const MyString& name, const Optional<MyString>& due_date, const MyString& description)
{
    for (int i = 0; i < tasks.getSize(); i++)
    {
        if (tasks[i].getName() == name && tasks[i].getDate() == due_date && tasks[i].getDescription() == description)
            throw std::logic_error("This task already exists!");
    }

    Task newTask(name, due_date, description);
    tasks.pushBack(newTask);
}

void User::addTask(const MyString& name, const MyString& description)
{
    for (int i = 0; i < tasks.getSize(); i++)
    {
        if (tasks[i].getName() == name && tasks[i].getDescription() == description)
            throw std::logic_error("This task already exists!");
    }

    Task newTask(name, description);
    tasks.pushBack(newTask);
}

void User::updateTaskName(unsigned id, const MyString& newName)
{
    Task& task = getTaskByID(id);
    task.setName(newName);
}

void User::updateTaskDescription(unsigned id, const MyString& newDescription)
{
    Task& task = getTaskByID(id);
    task.setDescription(newDescription);
}

void User::startTask(unsigned id)
{
    Task& task = getTaskByID(id);
    task.setStatus(Status::IN_PROCESS);
}

void User::finishTask(unsigned id)
{
    Task& task = getTaskByID(id);
    task.setStatus(Status::DONE);
}

void User::deleteTask(unsigned id)
{
    // If task is in dashboard, to remove it
    dashboard.removeTask(id);

    for (int i = 0; i < tasks.getSize(); i++)
    {
        if (tasks[i].getId() == id)
            tasks.popAt(i);
    }
}

void User::listTask(unsigned id)
{
    Task& task = getTaskByID(id);
    task.printTask();
}

void User::listTask(const MyString& name)
{
    Task& task = getTaskByName(name);
    task.printTask();
}

void User::addTaskToDashboard(unsigned id)
{
    Task& task = getTaskByID(id);

    if (task.getStatus() != Status::OVERDUE)
        dashboard.addTask(task);
}

void User::listDashboard() const
{
    dashboard.listTasks();
}

void User::removeTaskFromDashboard(unsigned id)
{
    dashboard.removeTask(id);
}

void User::listTasks(const Optional<MyString>& due_date) const
{
    for (int i = 0; i < tasks.getSize(); i++)
    {
        if(tasks[i].getDate() == due_date)
            tasks[i].printTask();
    }
}

void User::listCompletedTasks() const
{
    for (int i = 0; i < tasks.getSize(); i++)
    {
        if (tasks[i].getStatus() == Status::DONE)
            tasks[i].printTask();
    }
}

void User::listTasks() const
{
    for (int i = 0; i < tasks.getSize(); i++)
        tasks[i].printTask();
}
