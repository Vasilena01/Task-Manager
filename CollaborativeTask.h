#pragma once
#include "Task.h"

class CollaborativeTask : public Task
{
public:
    CollaborativeTask(const MyString& name, const Optional<std::tm>& due_date, const MyString& description, 
        const Optional<MyString>& assignee = Optional<MyString>());

    CollaborativeTask(const MyString& name, const MyString& description, 
        const Optional<MyString>& assignee = Optional<MyString>());

    Optional<MyString> getAssignee() const;
    void setAssignee(const MyString& assignee);

    Task* clone() const override;
    void printTask() const override;
private:
    Optional<MyString> assignee;
};

