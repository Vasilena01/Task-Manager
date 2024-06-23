#pragma once
#include "Session.h"
#include "MyString.h"
#include "GlobalConstants.h"

class Engine
{
public:
	void start();
private:
    Session session;

	void handleCommand(const MyString& command, bool& isRunning);
	void handleRegister(const MyString& username, const MyString& password);
    bool handleLogin(const MyString& username, const MyString& password);
    void handleLogout();
    void handleExit();
    void handleAddTask(const MyString& name, const std::tm& due_date, const MyString& description);
    void handleAddTask(const MyString& name, const MyString& description);
    void handleUpdateTaskName(unsigned id, const MyString& name);
    void handleStartTask(unsigned id);
    void handleUpdateTaskDescription(unsigned id, const MyString& description);
    void handleRemoveTaskFromDashboard(unsigned id);
    void handleAddTaskToDashboard(unsigned id);
    void handleDeleteTask(unsigned id);
    void handleListTasks(const std::tm& due_date);
    void handleListTasks();
    void handleListCompletedTasks();
    void handleListDashboard();
    void handleFinishTask(unsigned id);
    void handleAddCollaboration(const MyString& name);
    void handleDeleteCollaboration(const MyString& name);
    void handleListCollaborations();
    void handleAddUserToCollaboration(const MyString& collabName, const MyString& username);
    void handleAssignTask(const MyString& collabName, const MyString& username, const MyString& taskName, const std::tm& dueDate, const MyString& description);
    void handleListCollaborationTasks(const MyString& collabName);

    bool isDateFilled(const std::tm& date);
    bool isValidDate(const char* dateStr);
    std::tm parseDate(const char* dateStr);
};

