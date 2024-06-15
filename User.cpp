#include "User.h"

//Task& User::getTaskByID(unsigned id)
//{
//    for (int i = 0; i < tasks.getSize(); i++)
//    {
//        if (tasks[i] != nullptr && tasks[i]->getId() == id)
//            return *tasks[i];
//    }
//
//    throw std::logic_error("There is no task with the given ID!");
//}
//
//const Task& User::getTaskByID(unsigned id) const
//{
//    for (int i = 0; i < tasks.getSize(); i++)
//    {
//        if (tasks[i] != nullptr && tasks[i]->getId() == id)
//            return *tasks[i];
//    }
//
//    throw std::logic_error("There is no task with the given ID!");
//}
//
//Task& User::getTaskByName(const MyString& name)
//{
//    SharedPtr<Task> taskToReturn = nullptr;
//
//    for (int i = 0; i < tasks.getSize(); i++)
//    {
//        if (tasks[i] != nullptr && tasks[i]->getName() == name)
//        {
//            if (taskToReturn == nullptr || tasks[i]->getId() < taskToReturn->getId())
//            {
//                taskToReturn = tasks[i];
//            }
//        }
//    }
//
//    if (taskToReturn == nullptr)
//    {
//        throw std::runtime_error("No task found with the given name!");
//    }
//
//    return *taskToReturn;
//}

SharedPtr<Task> User::getTaskByID(unsigned id)
{
    for (int i = 0; i < tasks.getSize(); i++)
    {
        if (tasks[i] != nullptr && tasks[i]->getId() == id)
            return tasks[i];
    }

    throw std::logic_error("There is no task with the given ID!");
}

const SharedPtr<Task> User::getTaskByID(unsigned id) const
{
    for (int i = 0; i < tasks.getSize(); i++)
    {
        if (tasks[i] != nullptr && tasks[i]->getId() == id)
            return tasks[i];
    }

    throw std::logic_error("There is no task with the given ID!");
}

SharedPtr<Task> User::getTaskByName(const MyString& name)
{
    SharedPtr<Task> taskToReturn = nullptr;

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

void User::addTask(const MyString& name, const Optional<MyString>& due_date, const MyString& description)
{
    for (int i = 0; i < tasks.getSize(); i++)
    {
        if (tasks[i] != nullptr && tasks[i]->getName() == name && tasks[i]->getDate() == due_date && tasks[i]->getDescription() == description)
            throw std::logic_error("This task already exists!");
    }

    SharedPtr<Task> newTask(new Task(name, due_date, description)); 
    tasks.addTask(newTask);
    /*Task newTask(name, due_date, description);
    tasks.addTask(newTask);*/
    //tasks.pushBack(newTask);
}

void User::addTask(const MyString& name, const MyString& description)
{
    for (int i = 0; i < tasks.getSize(); i++)
    {
        if (tasks[i] != nullptr && tasks[i]->getName() == name && tasks[i]->getDescription() == description)
            throw std::logic_error("This task already exists!");
    }

    SharedPtr<Task> newTask(new Task(name, description)); 
    tasks.addTask(newTask);
    /*Task newTask(name, description);
    tasks.addTask(newTask);*/
    //tasks.pushBack(newTask);
}

void User::addTask(SharedPtr<Task> task)
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
    /*Task& task = getTaskByID(id);
    task.setName(newName);*/

    SharedPtr<Task> task = getTaskByID(id);
    task->setName(newName);
}

void User::updateTaskDescription(unsigned id, const MyString& newDescription)
{
    /*Task& task = getTaskByID(id);
    task.setDescription(newDescription);*/
    SharedPtr<Task> task = getTaskByID(id);
    task->setDescription(newDescription);
}

void User::startTask(unsigned id)
{
    /*Task& task = getTaskByID(id);
    task.setStatus(Status::IN_PROCESS);*/
    SharedPtr<Task> task = getTaskByID(id);
    task->setStatus(Status::IN_PROCESS);
}

void User::finishTask(unsigned id)
{
   /* Task& task = getTaskByID(id);
    task.setStatus(Status::DONE);*/
    SharedPtr<Task> task = getTaskByID(id);
    task->setStatus(Status::DONE);
}

void User::deleteTask(unsigned id)
{
    // If task is in dashboard, to remove it
    if(dashboard.isTaskInDashboard(id))
        dashboard.removeTask(id);

    for (int i = 0; i < tasks.getSize(); i++)
    {
        if (tasks[i] != nullptr && tasks[i]->getId() == id)
        {
            tasks[i] = nullptr;
            return;
        }
        //tasks.popAt(i);
    }
    throw std::logic_error("There is no task with the given ID!");
}

void User::listTask(unsigned id)
{
    /*Task& task = getTaskByID(id);
    task.printTask();*/
    SharedPtr<Task> task = getTaskByID(id);
    task->printTask();
}

void User::listTask(const MyString& name)
{
   /* Task& task = getTaskByName(name);
    task.printTask();*/
    SharedPtr<Task> task = getTaskByName(name);
    task->printTask();
}

void User::addTaskToDashboard(unsigned id)
{
    //Task& task = getTaskByID(id);
    SharedPtr<Task> task = getTaskByID(id);
    //task->printTask();

    if (task->getStatus() != Status::OVERDUE)
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
        if(tasks[i] != nullptr && tasks[i]->getDate() == due_date)
            tasks[i]->printTask();
    }
}

void User::listCompletedTasks() const
{
    for (int i = 0; i < tasks.getSize(); i++)
    {
        if (tasks[i] != nullptr && tasks[i]->getStatus() == Status::DONE)
            tasks[i]->printTask();
    }
}

void User::listTasks() const
{
    for (int i = 0; i < tasks.getSize(); i++)
    {
        if(tasks[i] != nullptr)
            tasks[i]->printTask();
    }
}
