#include "Task.h"

unsigned Task::nextId = 0;

Task::Task(const MyString& name, const Optional<std::tm>& due_date, const MyString& description) : Task(name, description)
{
	this->due_date = due_date;
}

Task::Task(const MyString& name, const MyString& description)
{
	this->id = nextId++;
	this->taskName = name;
	this->description = description;
	this->status = Status::ON_HOLD;
}

unsigned Task::getId() const
{
	return id;
}

MyString Task::getName() const
{
	return taskName;
}

Optional<std::tm> Task::getDate() const
{
	return due_date;
}

Status Task::getStatus() const
{
	return status;
}

MyString Task::getDescription() const
{
	return description;
}

void Task::setId(unsigned newId)
{
	id = newId;
}

void Task::setName(const MyString& newName)
{
	taskName = newName;
}

void Task::setDescription(const MyString& newDescription)
{
	this->description = newDescription;
}

void Task::setStatus(const Status& newStatus)
{
	this->status = newStatus;
}

void Task::printTask() const
{

	std::cout << "ID: " << id << std::endl;
	std::cout << "NAME: " << taskName << std::endl;

	if (due_date.isFilled())
	{
		printDate();
	}
	printStatus(status);

	std::cout << "DESCRIPTION: " << description << std::endl;
}

Task* Task::clone() const
{
	return new Task(*this);
}

void Task::printDate() const
{
	try {
		std::tm tm = due_date.getValue();
		std::time_t time = std::mktime(&tm);

		if (time != -1) {
			char buffer[1024];
			ctime_s(buffer, sizeof(buffer), &time);

			std::cout << "DATE: " << buffer;
		}
		else {
			std::cout << "DATE: Error converting date" << std::endl;
		}
	}
	catch (const std::exception& e) {
		std::cout << "DATE: " << e.what() << std::endl;
	}
}

void Task::printStatus(const Status& status) const
{
	std::cout << "STATUS: ";

	switch (status)
	{
	case Status::ON_HOLD: std::cout << "ON_HOLD"; break;
	case Status::IN_PROCESS: std::cout << "IN_PROCESS"; break;
	case Status::DONE: std::cout << "DONE"; break;
	case Status::OVERDUE: std::cout << "OVERDUE"; break;
	}
	std::cout << std::endl;
}

