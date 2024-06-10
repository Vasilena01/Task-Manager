#pragma once
#include <iostream>
#include <ctime>
#include "Optional.hpp"
#include "MyString.h"
#include "TaskStatus.h"
#include "DatePool.h"

class Task
{
public:
	Task() = default;
	Task(const MyString& name, const Optional<MyString>& due_date, const MyString& description);
	Task(const MyString& name, const MyString& description);

	unsigned getId() const;
	MyString getName() const;
	Optional<MyString> getDate() const;
	Status getStatus() const;
	MyString getDescription() const;

	void setName(const MyString& newName);
	void setDescription(const MyString& newDescription);
	void setStatus(const Status& newStatus);

	void printTask() const;
	//virtual ~Task() = default;
private:
	unsigned id;
	MyString taskName;
	Optional<MyString> due_date;
	Status status;
	MyString description;

	static unsigned nextId;

	void printDate() const;
	void printStatus(const Status& status) const;
};

