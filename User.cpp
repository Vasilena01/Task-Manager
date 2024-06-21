#include "User.h"

User::User() : username(MyString()), password(MyString())
{}

User::User(const MyString& username, const MyString& password) : username(username), password(password)
{}

MyString User::getUsername() const
{
    return username;
}

MyString User::getPassword() const
{
    return password;
}

Task* User::getTaskByID(unsigned id)
{
    for (int i = 0; i < tasks.getSize(); i++)
    {
        if (tasks[i] != nullptr && tasks[i]->getId() == id)
            return tasks[i];
    }

    throw std::logic_error("There is no task with the given ID!");
}

Task* User::getTaskByName(const MyString& name)
{
    Task* taskToReturn = nullptr;

    for (int i = 0; i < tasks.getSize(); i++)
    {
        if (tasks[i] != nullptr && tasks[i]->getName() == name)
        {
            if (taskToReturn == nullptr || tasks[i]->getId() < taskToReturn->getId())
            {
                taskToReturn = tasks[i];
            }
        }
    }

    if (taskToReturn == nullptr)
    {
        throw std::runtime_error("No task found with the given name!");
    }

    return taskToReturn;
}

void User::addTask(const MyString& name, const Optional<std::tm>& due_date, const MyString& description)
{
    for (int i = 0; i < tasks.getSize(); i++)
    {
        if (tasks[i] != nullptr && tasks[i]->getName() == name 
            && GlobalFunctions::compareDates(tasks[i]->getDate(), due_date.getValue())
            && tasks[i]->getDescription() == description)
        {
            throw std::logic_error("This task already exists!");
        }
    }

    Task newTask(name, due_date, description); 
    newTask.printTask();
    tasks.addTask(newTask);
}

void User::addTask(const MyString& name, const MyString& description)
{
    for (int i = 0; i < tasks.getSize(); i++)
    {
        if (tasks[i] != nullptr && tasks[i]->getName() == name && tasks[i]->getDescription() == description)
            throw std::logic_error("This task already exists!");
    }

    Task newTask(name, description);
    tasks.addTask(newTask);
}

void User::addTask(Task* task)
{
    for (size_t i = 0; i < tasks.getSize(); ++i)
    {
        if (tasks[i] != nullptr && tasks[i]->getName() == task->getName() && tasks[i]->getDescription() == task->getDescription())
        {
            throw std::logic_error("This task already exists!");
        }
    }

    tasks.addTask(task);
}

void User::updateTaskName(unsigned id, const MyString& newName)
{
    Task* task = getTaskByID(id);
    task->setName(newName);
}

void User::updateTaskDescription(unsigned id, const MyString& newDescription)
{
    Task* task = getTaskByID(id);
    task->setDescription(newDescription);
}

void User::startTask(unsigned id)
{
    Task* task = getTaskByID(id);
    task->setStatus(Status::IN_PROCESS);
}

void User::finishTask(unsigned id)
{
    Task* task = getTaskByID(id);
    task->setStatus(Status::DONE);
}

void User::deleteTask(unsigned id)
{
    // If task is in dashboard, to remove it
    if(dashboard.isTaskInDashboard(id))
        dashboard.removeTask(id);

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
    //throw std::logic_error("There is no task with the given ID!");
}

void User::listTask(unsigned id)
{
    Task* task = getTaskByID(id);
    task->printTask();
}

void User::listTask(const MyString& name)
{
    Task* task = getTaskByName(name);
    task->printTask();
}

void User::addTaskToDashboard(unsigned id)
{
    Task* task = getTaskByID(id);

    if (task->getStatus() != Status::OVERDUE)
        dashboard.addTask(task->clone());
}

void User::listDashboard()
{
    dashboard.listTasks();
}

void User::removeTaskFromDashboard(unsigned id)
{
    dashboard.removeTask(id);
}

void User::listTasks(const Optional<std::tm>& due_date)
{
    for (int i = 0; i < tasks.getSize(); i++)
    {
        if(tasks[i] != nullptr 
            && GlobalFunctions::compareDates(tasks[i]->getDate(), due_date.getValue())) {
            tasks[i]->printTask();
        }
    }
}

void User::listCompletedTasks()
{
    for (int i = 0; i < tasks.getSize(); i++)
    {
        if (tasks[i] != nullptr && tasks[i]->getStatus() == Status::DONE)
            tasks[i]->printTask();
    }
}

void User::listTasks()
{
    if (tasks.getSize())
    {
        for (int i = 0; i < tasks.getSize(); i++)
        {
            if (tasks[i] != nullptr)
                tasks[i]->printTask();
        }
    }
    throw std::runtime_error("This user has no tasks!");
}

TasksCollection User::getTasks() const
{
    return tasks;
}

Dashboard User::getDashboard() const
{
    return dashboard;
}

