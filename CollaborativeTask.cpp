#include "CollaborativeTask.h"

CollaborativeTask::CollaborativeTask(const MyString& name, const Optional<MyString>& due_date, 
	const MyString& description, const Optional<MyString>& assignee) : Task(name, due_date, description), assignee(assignee) {}

CollaborativeTask::CollaborativeTask(const MyString & name, const MyString & description, 
	const Optional<MyString>&assignee) : Task(name, description), assignee(assignee) {}

Optional<MyString> CollaborativeTask::getAssignee() const
{
	return assignee;
}

void CollaborativeTask::setAssignee(const MyString& assignee)
{
	this->assignee = assignee;
}

SharedPtr<Task> CollaborativeTask::clone() const
{
	SharedPtr<Task> newTask(new CollaborativeTask(*this));
	return newTask;
}
