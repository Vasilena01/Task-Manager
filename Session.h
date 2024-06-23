#pragma once
#include "GlobalConstants.h"
#include "Collaboration.h"
#include "CollaborativeTask.h"

class Session
{
public:
	Session();
	~Session();

	// User functionalities
	void registerUser(const MyString& username, const MyString& password);
	bool login(const MyString& username, const MyString& password);
	void logout();
	void exit();

	// Collaboration functionalities
	void addCollaboration(const MyString& name);
	void deleteCollaboration(const MyString& name);
	void listCollaborations() const;
	void addUserToCollaboration(const MyString& collabName, const MyString& username);
	void assignTaskInCollaboration(const MyString& collabName, const MyString& username, 
		const MyString& name, const std::tm& dueDate, const MyString& description);
	void listCollabTasks(const MyString& name);
	void deleteTaskFromAllCollaborations(unsigned id);

	User* getCurrentUser() const;
private:
	User* currentUser;
	Vector<User*> allUsers;
	Vector<Collaboration*> allCollaborations;
	TasksCollection allTasks;

	void loadUsers(const char* fileName);
	void saveUsers(const char* fileName) const;
	void loadCollaborations(const MyString& fileName);
	void saveCollaborations(const MyString& fileName) const;

	std::tm getCurrentDate();
	bool areDatesEqual(const std::tm& date1, const std::tm& date2);

	// Getters
	User* getUserByUsername(const MyString& name);
	Task* getTaskById(unsigned id);
	Collaboration* getCollaborationByName(const MyString& name);
	int getCollabIndexById(unsigned id);
};

