#pragma once
#include "User.h"
#include "TasksCollection.h"
#include "Vector.hpp"
#include "MyString.h"

class Collaboration
{
public:
    Collaboration() = default;
    Collaboration(const MyString& name, User* creator);

    unsigned getId() const;
    MyString getName() const;
    User* getCreator() const;
    Vector<User*> getUsers();
    TasksCollection getTasks();

    void setId(unsigned newId);
    void setName(const MyString& newName);
    void setCreator(User* newCreator);

    bool isUserInCollaboration(const MyString& username) const;
    bool isTaskInCollaboration(unsigned id);
    void addUser(User* user);
    void addTask(const Task& task);    
    void addTask(Task* task);
    void deleteTasksInCollaboration();
    void deleteTaskFromAllUsers(unsigned taskId);
    void listTasks() const;
private:
    unsigned collabId;
    MyString name;
    User* creator;
    Vector<User*> users;
    TasksCollection tasks;

    static unsigned nexCollabtId;
};
