#pragma once
#include "Task.h"

class CollaborativeTask : public Task
{
public:
    CollaborativeTask(const MyString& name, const Optional<MyString>& due_date, const MyString& description, 
        const Optional<MyString>& assignee = Optional<MyString>());

    CollaborativeTask(const MyString& name, const MyString& description, 
        const Optional<MyString>& assignee = Optional<MyString>());

    Optional<MyString> getAssignee() const;
    void setAssignee(const MyString& assignee);

    virtual SharedPtr<Task> clone() const override;
private:
    Optional<MyString> assignee;
};

