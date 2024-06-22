#pragma once
#include "GlobalFunctions.h"
#include "MyString.h"
#include "Vector.hpp"
#include "Task.h"
#include "Dashboard.h"
#include "TasksCollection.h"
#include "Collaboration.h"

class User
{
public:
	User();
	User(const User& other);
	User& operator=(const User& other);

	User(const MyString& username, const MyString& password);
	MyString getUsername() const;
	MyString getPassword() const;

	Task* getTaskByID(unsigned id);
	Task* getTaskByName(const MyString& name);

	void addTask(const MyString& name, const Optional<std::tm>& due_date, const MyString& description);
	void addTask(const MyString& name, const MyString& description);
	void addTask(Task* task);
	void updateTaskName(unsigned id, const MyString& newName);
	void updateTaskDescription(unsigned id, const MyString& newDescription);
	void startTask(unsigned id);
	void finishTask(unsigned id);
	void deleteTask(unsigned id);

	void addTaskToDashboard(unsigned id);
	void removeTaskFromDashboard(unsigned id);

	void listTask(unsigned id);
	void listTask(const MyString& name);
	void listTasks(const Optional<std::tm>& due_date);
	void listCompletedTasks();
	void listTasks();
	void listDashboard();

	TasksCollection getTasks() const;
	Dashboard getDashboard() const;
private:
	MyString username;
	MyString password;
	TasksCollection tasks;
	Dashboard dashboard;
};

