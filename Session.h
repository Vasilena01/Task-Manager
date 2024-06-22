#pragma once
#include "Vector.hpp"
#include "MyString.h"
#include "UniquePtr.hpp"
#include "GlobalConstants.h"
#include "DatePool.h"
#include "User.h"
#include "Collaboration.h"
#include "TasksCollection.h"

class Session
{
public:
	Session();
	~Session();

	void registerUser(const MyString& username, const MyString& password);
	bool login(const MyString& username, const MyString& password);
	void logout();
	void exit();

	User* getCurrentUser() const;
	Vector<User> getAllUsers() const;
private:
	UniquePtr<User> currentUser;
	Vector<User> allUsers;
	//Vector<Collaboration> allCollaborations;

	void loadUsers(const char* fileName);
	void saveUsers(const char* fileName) const;
	void saveTasks(std::ofstream& file, TasksCollection& currUserTasks, size_t taskCount);
	//void loadTasks(const MyString& fileName) const;
	/*void loadCollaborations(const MyString& fileName);
	void saveCollaborations(const MyString& fileName) const;*/

	std::tm getCurrentDate();
	bool areDatesEqual(const std::tm& date1, const std::tm& date2);
	int findUserIndexByUsername(const MyString& username);
};

