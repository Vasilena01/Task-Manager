#include "TasksCollection.h"

TasksCollection::TasksCollection()
{
	capacity = 8;
	data = new Task * [capacity] {nullptr};
	size = 0;
}

TasksCollection::TasksCollection(const TasksCollection& other)
{
	copyFrom(other);
}

TasksCollection& TasksCollection::operator=(const TasksCollection& other)
{
	if (this != &other)
	{
		free();
		copyFrom(other);
	}
	return *this;
}

TasksCollection::TasksCollection(TasksCollection&& other) noexcept
{
	moveFrom(std::move(other));
}

TasksCollection& TasksCollection::operator=(TasksCollection&& other) noexcept
{
	if (this != &other)
	{
		free();
		moveFrom(std::move(other));
	}
	return *this;
}

void TasksCollection::addTask(const MyString& name, const MyString& due_date, const MyString& description)
{
	if (size == capacity)
		resize(capacity * 2);

	//data[size++] = Task(name, due_date, description);
}

void TasksCollection::addTask(Task* task)
{
	if (size == capacity)
		resize(capacity * 2);

	data[size++] = task;
	task = nullptr;
}

void TasksCollection::addTask(const Task& task)
{
	//Task* cloned = task.clone();
	//addTask(cloned);
}

TasksCollection::~TasksCollection()
{
	free();
}

size_t TasksCollection::getSize() const
{
	return size;
}

const Task* TasksCollection::operator[](unsigned index) const
{
	return data[index];
}

void TasksCollection::free()
{
	for (int i = 0; i < size; i++)
		delete data[i];
	delete[] data;
}

void TasksCollection::copyFrom(const TasksCollection& other)
{
	data = new Task * [other.capacity];

	for (int i = 0; i < other.size; i++)
		//data[i] = other.data[i]->clone();

	size = other.size;
	capacity = other.capacity;
}

void TasksCollection::moveFrom(TasksCollection&& other)
{
	data = other.data;
	other.data = nullptr;

	size = other.size;
	capacity = other.capacity;

	other.size = other.capacity = 0;
}

void TasksCollection::resize(unsigned newCap)
{
	Task** newData = new Task * [newCap];
	for (int i = 0; i < size; i++)
		newData[i] = data[i];
	delete[] data;
	data = newData;
	capacity = newCap;
}
