#pragma once
#include "Task.h"

class TasksCollection
{
	TasksCollection();
	TasksCollection(const TasksCollection& other);
	TasksCollection& operator=(const TasksCollection& other);
	~TasksCollection();

	TasksCollection(TasksCollection&& other) noexcept;
	TasksCollection& operator=(TasksCollection&& other) noexcept;

	void addTask(const MyString& name, const MyString& due_date, const MyString& description);
	void addTask(Task* task);
	void addTask(const Task& task);

	size_t getSize() const;

	const Task* operator[](unsigned index) const;
private:
	Task** data;
	size_t size;
	size_t capacity;

	void free();
	void copyFrom(const TasksCollection& other);
	void moveFrom(TasksCollection&& other);
	void resize(unsigned newCap);
};

