#include "Collaboration.h"

unsigned Collaboration::nexCollabtId = 0;

Collaboration::Collaboration(const MyString& name, User* creator) : name(name), creator(creator)
{
    collabId = nexCollabtId++;
}

unsigned Collaboration::getId() const
{
    return collabId;
}

MyString Collaboration::getName() const
{
    return name;
}

User* Collaboration::getCreator() const
{
    return creator;
}

Vector<User*> Collaboration::getUsers()
{
    return users;
}

TasksCollection Collaboration::getTasks()
{
    return tasks;
}

void Collaboration::setId(unsigned newId)
{
    collabId = newId;
}

void Collaboration::setName(const MyString& newName)
{
    name = newName;
}

void Collaboration::setCreator(User* newCreator)
{
    creator = newCreator;
}

bool Collaboration::isUserInCollaboration(const MyString& username) const
{
    for (int i = 0; i < users.getSize(); i++)
    {
        if (users[i]->getUsername() == username)
            return true;
    }
    return creator->getUsername() == username;
}

void Collaboration::addUser(User* user)
{
    users.pushBack(user);
}

void Collaboration::addTask(const Task& task)
{
    tasks.addTask(task);
}

void Collaboration::addTask(Task* task)
{
    tasks.addTask(task);
}

void Collaboration::deleteTasksInCollaboration()
{
    for (int i = 0; i < tasks.getSize(); i++)
    {
        unsigned taskId = tasks[i]->getId();
        deleteTaskFromAllUsers(taskId);
        tasks.removeTask(taskId);
    }
}

void Collaboration::deleteTaskFromAllUsers(unsigned taskId)
{
    for (int i = 0; i < users.getSize(); i++)
    {
        if (users[i]->getTaskByID(taskId) != nullptr)
        {
            users[i]->deleteTask(taskId);
        }
    }
}

void Collaboration::listTasks() const
{
    for (int i = 0; i < tasks.getSize(); i++)
    {
        tasks[i]->printTask();
    }
}
