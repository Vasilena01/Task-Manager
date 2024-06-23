#pragma once
#include <iostream>
#include <ctime>
#include "Optional.hpp"
#include "MyString.h"
#include "TaskStatus.h"

class Task
{
public:
	Task() = default;
	Task(const MyString& name, const Optional<std::tm>& due_date, const MyString& description);
	Task(const MyString& name, const MyString& description);

	unsigned getId() const;
	MyString getName() const;
	Optional<std::tm> getDate() const;
	Status getStatus() const;
	MyString getDescription() const;

	void setId(unsigned newId);
	void setName(const MyString& newName);
	void setDescription(const MyString& newDescription);
	void setStatus(const Status& newStatus);

	virtual void printTask() const;
	virtual Task* clone() const;
	virtual ~Task() = default;
protected:
	unsigned id;
	MyString taskName;
	Optional<std::tm> due_date;
	Status status;
	MyString description;

	static unsigned nextId;

	void printDate() const;
	void printStatus(const Status& status) const;
};

