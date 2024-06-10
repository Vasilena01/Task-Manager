#pragma once
#include "MyString.h"
#include "Task.h"
#include "Dashboard.h"
//#include "Container.hpp"
#include "Vector.hpp"

class User
{
public:
	Task& getTaskByID(unsigned id);
	const Task& getTaskByID(unsigned id) const;
	Task& getTaskByName(const MyString& name);

	void addTask(const MyString& name, const Optional<MyString>& due_date, const MyString& description);
	void addTask(const MyString& name, const MyString& description);
	void updateTaskName(unsigned id, const MyString& newName);
	void updateTaskDescription(unsigned id, const MyString& newDescription);
	void startTask(unsigned id);
	void finishTask(unsigned id);
	void deleteTask(unsigned id);

	void addTaskToDashboard(unsigned id);
	void removeTaskFromDashboard(unsigned id);

	void listTask(unsigned id);
	void listTask(const MyString& name);
	void listTasks(const Optional<MyString>& due_date) const;
	void listCompletedTasks() const;
	void listTasks() const;
	void listDashboard() const;
private:
	MyString username;
	MyString password;
	Vector<Task> tasks;
	Dashboard dashboard;
};

