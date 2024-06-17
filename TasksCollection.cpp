#include "TasksCollection.h"

TasksCollection::TasksCollection()
{
	capacity = 8;
	data = new SharedPtr<Task> [capacity] {nullptr};
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

void TasksCollection::addTask(SharedPtr<Task> task)
{
	if (size == capacity)
		resize(capacity * 2);

	data[size++] = task;
}

void TasksCollection::addTask(const Task& task)
{
	SharedPtr<Task> cloned = task.clone();
	addTask(cloned);
}

TasksCollection::~TasksCollection()
{
	free();
}

size_t TasksCollection::getSize() const
{
	return size;
}

SharedPtr<Task>& TasksCollection::operator[](unsigned index)
{
	if (index >= size) {
		throw std::out_of_range("Index out of range");
	}

	return data[index];
}

const SharedPtr<Task> TasksCollection::operator[](unsigned index) const
{
	if (index >= size) {
		throw std::out_of_range("Index out of range");
	}

	return data[index];
}

void TasksCollection::free()
{
	// shared_ptr will automatically handle memory management, no need to delete manually
	
	delete[] data;
	data = nullptr;
	size = 0;
	capacity = 0;
}

void TasksCollection::copyFrom(const TasksCollection& other)
{
	data = new SharedPtr<Task> [other.capacity];

	for (int i = 0; i < other.size; i++)
		data[i] = SharedPtr<Task>(other.data[i]->clone());

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
	SharedPtr<Task>* newData = new SharedPtr<Task>[newCap];
	for (int i = 0; i < size; i++)
		newData[i] = data[i];
	delete[] data;
	data = newData;
	capacity = newCap;
}
