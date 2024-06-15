#pragma once
#include "Task.h"
#include "SharedPtr.hpp"

class TasksCollection
{
public:
	TasksCollection();
	TasksCollection(const TasksCollection& other);
	TasksCollection& operator=(const TasksCollection& other);
	~TasksCollection();

	TasksCollection(TasksCollection&& other) noexcept;
	TasksCollection& operator=(TasksCollection&& other) noexcept;

	//void addTask(const MyString& name, const MyString& due_date, const MyString& description);
	void addTask(const SharedPtr<Task>& task);
	//void addTask(const Task& task);

	size_t getSize() const;

	SharedPtr<Task>& operator[](unsigned index);
	const SharedPtr<Task> operator[](unsigned index) const;
private:
	SharedPtr<Task>* data;
	size_t size;
	size_t capacity;

	void free();
	void copyFrom(const TasksCollection& other);
	void moveFrom(TasksCollection&& other);
	void resize(unsigned newCap);
};

