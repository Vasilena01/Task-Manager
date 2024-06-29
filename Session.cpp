#include "Session.h"
#include "fstream";

Session::Session()
{
	loadUsers("users.dat");
	loadCollaborations("collaborations.dat");
}

Session::Session(const Session& other)
{
	copyFrom(other);
}

Session& Session::operator=(const Session& other)
{
	if (this != &other)
	{
		free();
		copyFrom(other);
	}
	return *this;
}

Session::Session(Session&& other) noexcept
{
	moveFrom(std::move(other));
}

Session& Session::operator=(Session&& other) noexcept
{
	if (this != &other)
	{
		free();
		moveFrom(std::move(other));
	}
	return *this;
}

Session::~Session() noexcept
{
	free();
	exit();
}

void Session::registerUser(const MyString& username, const MyString& password)
{
	for (size_t i = 0; i < allUsers.getSize(); i++)
	{
		if (allUsers[i]->getUsername() == username)
		{
			std::cerr << "User already exists." << std::endl;
			return;
		}
	}

	User* newUser = new User(username, password);
	allUsers.pushBack(newUser);

	std::cout << "Registered successfully!" << std::endl;
}

bool Session::login(const MyString& username, const MyString& password)
{
	for (size_t i = 0; i < allUsers.getSize(); i++)
	{
		if (allUsers[i]->getUsername() == username && allUsers[i]->getPassword() == password)
		{
			currentUser = allUsers[i];
			std::cout << "Welcome back, " << username << "!" << std::endl;
			return true;
		}
	}
	std::cerr << "Invalid username or password." << std::endl;
	return false;
}

void Session::logout()
{
	if (currentUser)
	{
		std::cout << "Logged out: " << currentUser->getUsername() << std::endl;
		currentUser = nullptr;
	}
}

void Session::exit()
{

	if (currentUser)
	{
		currentUser = nullptr;
	}

	try
	{
		saveUsers("users.dat");
		saveCollaborations("collaborations.dat");
	}
	catch (std::exception& e)
	{
		std::cout << e.what();
	}

	std::cout << "Session ended. All data saved." << std::endl;
}

// Collaborations methods
void Session::addCollaboration(const MyString& name)
{
	try
	{
		if (getCollaborationByName(name)->getId())
			std::cout << "A collaboration with this name already exists!" << std::endl;
	}
	catch (std::exception& e)
	{
		Collaboration* newCollaboration = new Collaboration(name, currentUser);
		allCollaborations.pushBack(newCollaboration);
		std::cout << "Collaboration added successfully!" << std::endl;
	}
}

void Session::deleteCollaboration(const MyString& name)
{
	Collaboration* collaboration = getCollaborationByName(name);
	if (currentUser->getUsername() != collaboration->getCreator()->getUsername())
		throw std::logic_error("Only the creator of the collaboration can delete it!");

	// First dealing with deleting all tasks in collab and in its usersTasks collection
	collaboration->deleteTasksInCollaboration();

	// Dealing with deleting the whole collaboration
	int indexInCollabsArr = getCollabIndexById(collaboration->getId());
	allCollaborations.popAt(indexInCollabsArr);
}

void Session::listCollaborations() const
{
	for (int i = 0; i < allCollaborations.getSize(); i++)
	{
		// Checking if the currentUser is the creator or is part of the collabUsers
		if (allCollaborations[i]->isUserInCollaboration(currentUser->getUsername()))
		{
			std::cout << allCollaborations[i]->getName() << std::endl;
		}
	}
}

void Session::addUserToCollaboration(const MyString& collabName, const MyString& username)
{
	Collaboration* collaboration = getCollaborationByName(collabName);
	User* newUser = getUserByUsername(username);
	collaboration->addUser(newUser);
}

void Session::assignTaskInCollaboration(const MyString& collabName, const MyString& username, const MyString& name, const std::tm& dueDate, const MyString& description)
{
	Collaboration* collaboration = getCollaborationByName(collabName);
	User* assignee = getUserByUsername(username);

	Task* taskForUser = new CollaborativeTask(name, dueDate, description, username);

	collaboration->addTask(taskForUser);
	collaboration->addUser(assignee);
	assignee->addTask(taskForUser);
}

void Session::listCollabTasks(const MyString& name)
{
	Collaboration* collaboration = getCollaborationByName(name);
	collaboration->listTasks();
}

User* Session::getCurrentUser() const
{
	return currentUser;
}

void Session::loadUsers(const char* fileName)
{
	std::ifstream file(fileName, std::ios::binary | std::ios::in);
	if (!file.is_open())
		throw std::runtime_error("Failed to open file for reading!");

	std::tm today = getCurrentDate();

	while (true)
	{
		// Read username
		size_t usernameLength;
		file.read((char*)(&usernameLength), sizeof(usernameLength));

		char* username = new char[usernameLength + 1];
		file.read(username, usernameLength);
		username[usernameLength] = '\0';

		// Read password
		size_t passLength;
		file.read((char*)(&passLength), sizeof(passLength));

		char* password = new char[passLength + 1];
		file.read(password, passLength);
		password[passLength] = '\0';

		if (file.eof()) break;

		MyString newUsername = MyString(username);
		MyString newPassword = MyString(password);

		User* user = new User(newUsername, newPassword);
		delete[] username;
		delete[] password;

		// Read number of tasks
		size_t taskCount;
		file.read((char*)(&taskCount), sizeof(taskCount));

		// Read tasks
		for (unsigned i = 0; i < taskCount; i++) {
			// Read task ID
			unsigned taskId = 0;
			file.read((char*)(&taskId), sizeof(taskId));

			// Read task name
			size_t nameLength = 0;
			file.read((char*)(&nameLength), sizeof(nameLength));

			char* taskName = new char[nameLength + 1];
			file.read(taskName, nameLength);
			taskName[nameLength] = '\0';

			// Read due date
			bool hasDueDate = 0;
			file.read((char*)(&hasDueDate), sizeof(hasDueDate));
			std::tm dueDate;
			if (hasDueDate) {
				file.read((char*)&dueDate, sizeof(dueDate));
			}

			// Read status
			Status taskStatus;
			file.read((char*)(&taskStatus), sizeof(taskStatus));

			// Read description
			size_t descriptionLength;
			file.read((char*)(&descriptionLength), sizeof(descriptionLength));

			char* description = new char[descriptionLength + 1];
			file.read(description, descriptionLength);
			description[descriptionLength] = '\0';

			Task* task;
			if (hasDueDate)
				task = new Task(MyString(taskName), dueDate, MyString(description));
			else
				task = new Task(MyString(taskName), MyString(description));

			task->setId(taskId);
			task->setStatus(taskStatus);
			task->setDescription(description);
			user->addTask(task);
			allTasks.addTask(task);

			// Checks if the task's due_date is today and adds it to the dashboard
			if (hasDueDate) {
				std::tm taskDate = task->getDate().getValue();
				if (areDatesEqual(taskDate, today)) {
					user->addTaskToDashboard(task->getId());
				}
			}

			delete[] taskName;
			delete[] description;
		}

		// Read number of dashboard tasks
		size_t dashboardTaskCount;
		file.read((char*)(&dashboardTaskCount), sizeof(dashboardTaskCount));

		// Read and add dashboard tasks
		for (size_t i = 0; i < dashboardTaskCount; i++) {
			unsigned dashboardTaskId;
			file.read((char*)(&dashboardTaskId), sizeof(dashboardTaskId));
			user->addTaskToDashboard(dashboardTaskId);
		}

		allUsers.pushBack(user);
	}
}

void Session::saveUsers(const char* fileName) const
{
	std::ofstream file(fileName, std::ios::binary | std::ios::out);
	if (!file.is_open())
		throw std::runtime_error("Failed to open file for writing!");

	for (int i = 0; i < allUsers.getSize(); i++) {
		// Write username
		const MyString& username = allUsers[i]->getUsername();
		size_t usernameSize = username.getSize();
		file.write((const char*)&usernameSize, sizeof(usernameSize));
		file.write(username.c_str(), usernameSize);

		// Write password
		const MyString& password = allUsers[i]->getPassword();
		size_t passwordSize = password.getSize();
		file.write((const char*)&passwordSize, sizeof(passwordSize));
		file.write(password.c_str(), passwordSize);

		// Write number of tasks
		size_t taskCount = allUsers[i]->getTasks().getSize();
		file.write((const char*)(&taskCount), sizeof(taskCount));

		TasksCollection currUserTasks = allUsers[i]->getTasks();
		for (size_t j = 0; j < taskCount; j++)
		{
			Task* task = currUserTasks[j];

			// Write task ID
			unsigned taskId = task->getId();
			file.write((const char*)(&taskId), sizeof(taskId));

			// Write task name
			const MyString& name = task->getName();
			size_t nameLength = name.getSize();
			file.write((const char*)(&nameLength), sizeof(nameLength));
			file.write(name.c_str(), nameLength);

			// Write due date - handling if the task has/not a due_date
			bool hasDueDate = 0;
			if (task->getDate().isFilled())
			{
				std::tm due_date = task->getDate().getValue();
				hasDueDate = 1;
				file.write((const char*)&hasDueDate, sizeof(hasDueDate));
				file.write((const char*)&due_date, sizeof(due_date));
			}
			else
			{
				file.write((const char*)&hasDueDate, sizeof(hasDueDate));
			}

			// Write task status
			Status taskStatus = task->getStatus();
			file.write((const char*)(&taskStatus), sizeof(taskStatus));

			// Write task description
			const MyString& description = task->getDescription();
			size_t descriptionLength = description.getSize();
			file.write((const char*)(&descriptionLength), sizeof(descriptionLength));
			file.write(description.c_str(), descriptionLength);
		}

		// Write number of dashboard tasks
		TasksCollection currUserDashboardTasks = allUsers[i]->getDashboard().getTasks();
		size_t dashboardTaskCount = currUserDashboardTasks.getSize();
		file.write((const char*)(&dashboardTaskCount), sizeof(dashboardTaskCount));

		// Write dashboard task IDs
		for (size_t j = 0; j < dashboardTaskCount; j++) {
			const Task* dashboardTask = currUserDashboardTasks[j];

			unsigned dashboardTaskId = dashboardTask->getId();
			file.write((const char*)(&dashboardTaskId), sizeof(dashboardTaskId));
		}
	}
}

void Session::loadCollaborations(const MyString& fileName)
{
	std::ifstream file(fileName.c_str(), std::ios::binary | std::ios::in);
	if (!file.is_open())
		throw std::runtime_error("Failed to open file for reading!");

	while (true) {
		// Read ID
		unsigned collabId = 0;
		file.read((char*)(&collabId), sizeof(collabId));

		if (file.eof()) break;

		// Read name
		size_t nameLength = 0;
		file.read((char*)(&nameLength), sizeof(nameLength));

		char* collabName = new char[nameLength + 1];
		file.read(collabName, nameLength);
		collabName[nameLength] = '\0';

		// Read creator username
		size_t creatorNameLength = 0;
		file.read((char*)(&creatorNameLength), sizeof(creatorNameLength));

		char* creatorUsername = new char[creatorNameLength + 1];
		file.read(creatorUsername, creatorNameLength);
		creatorUsername[creatorNameLength] = '\0';
		User* creator = getUserByUsername(creatorUsername);

		// Creating a new collab based on collabName and the creator
		Collaboration* collab = new Collaboration(MyString(collabName), creator);

		// Read Users
		size_t usernamesCount;
		file.read((char*)(&usernamesCount), sizeof(usernamesCount));
		for (size_t i = 0; i < usernamesCount; i++) {
			size_t userNameLength = 0;
			file.read((char*)(&userNameLength), sizeof(userNameLength));

			char* username = new char[userNameLength + 1];
			file.read(username, userNameLength);
			username[userNameLength] = '\0';

			User* user = getUserByUsername(username);
			collab->addUser(user);

			delete[] username;
		}

		// Read Collab Task-ids
		size_t taskIdsCount;
		file.read((char*)(&taskIdsCount), sizeof(taskIdsCount));
		for (size_t i = 0; i < taskIdsCount; i++) {
			unsigned taskId = 0;
			file.read((char*)(&taskId), sizeof(taskId));

			Task* task = getTaskById(taskId);

			if (task) {
				collab->addTask(task);
			}
		}

		delete[] collabName;
		delete[] creatorUsername;

		allCollaborations.pushBack(collab);
	}
}

void Session::saveCollaborations(const MyString& fileName) const
{
	std::ofstream file(fileName.c_str(), std::ios::binary | std::ios::out);
	if (!file.is_open())
		throw std::runtime_error("Failed to open file for writing!");

	for (int i = 0; i < allCollaborations.getSize(); i++) {
		// Write collab Id
		unsigned collabId = allCollaborations[i]->getId();
		file.write((const char*)&collabId, sizeof(collabId));

		// Write collab name
		const MyString& collabName = allCollaborations[i]->getName();
		size_t nameLength = collabName.getSize();
		file.write((const char*)(&nameLength), sizeof(nameLength));
		file.write(collabName.c_str(), nameLength);

		// Write creator username
		const MyString& creatorName = allCollaborations[i]->getCreator()->getUsername();
		size_t creatorNameLength = creatorName.getSize();
		file.write((const char*)(&creatorNameLength), sizeof(creatorNameLength));
		file.write(creatorName.c_str(), creatorNameLength);

		// Write collab users
		Vector<User*> collabUsers = allCollaborations[i]->getUsers();
		size_t usernamesCount = collabUsers.getSize();
		file.write((const char*)&usernamesCount, sizeof(usernamesCount));

		for (int j = 0; j < usernamesCount; j++) {
			const MyString& username = collabUsers[j]->getUsername();
			size_t usernameLength = username.getSize();
			file.write((const char*)(&usernameLength), sizeof(usernameLength));
			file.write(username.c_str(), usernameLength);
		}

		// Write collab Tasks
		TasksCollection collabTasks = allCollaborations[i]->getTasks();
		size_t taskIdsCount = collabTasks.getSize();
		file.write((const char*)&taskIdsCount, sizeof(taskIdsCount));

		for (int j = 0; j < taskIdsCount; j++) {
			unsigned taskId = collabTasks[j]->getId();
			file.write((const char*)&taskId, sizeof(taskId));
		}
	}
}

std::tm Session::getCurrentDate()
{
	std::time_t t = std::time(nullptr);
	std::tm now = {};

#if defined(_WIN32) || defined(_WIN64)
	// Windows specific code using localtime_s
	errno_t err = localtime_s(&now, &t);
	if (err)
	{
		throw std::runtime_error("Failed to get local time");
	}
#else
	if (localtime_r(&t, &now) == nullptr)
	{
		throw std::runtime_error("Failed to get local time");
	}
#endif
	now.tm_hour = 0;
	now.tm_min = 0;
	now.tm_sec = 0;
	now.tm_isdst = -1;

	return now;
}

bool Session::areDatesEqual(const std::tm& date1, const std::tm& date2) {
	return (date1.tm_year == date2.tm_year && date1.tm_mon == date2.tm_mon && date1.tm_mday == date2.tm_mday);
}

User* Session::getUserByUsername(const MyString& name)
{
	bool isFound = 0;
	for (int i = 0; i < allUsers.getSize(); i++)
	{
		if (allUsers[i]->getUsername() == name)
		{
			isFound = 1;
			return allUsers[i];
		}
	}

	if (!isFound)
		throw std::logic_error("No user with the given username found!");
}

Task* Session::getTaskById(unsigned id)

{
	return allTasks.getTaskById(id);
}

Collaboration* Session::getCollaborationByName(const MyString& name)
{
	bool isFound = 0;
	for (int i = 0; i < allCollaborations.getSize(); i++)
	{
		if (allCollaborations[i]->getName() == name)
			return allCollaborations[i];
	}

	if (!isFound)
		throw std::logic_error("No collaboration with the given name was found!");
}

int Session::getCollabIndexById(unsigned id)
{
	for (int i = 0; i < allCollaborations.getSize(); i++)
	{
		if (allCollaborations[i]->getId() == id)
			return i;
	}
	return -1;
}

void Session::copyFrom(const Session& other)
{
	currentUser = other.currentUser ? new User(*other.currentUser) : nullptr;

	for (size_t i = 0; i < other.allUsers.getSize(); ++i)
	{
		allUsers.pushBack(new User(*other.allUsers[i]));
	}

	for (size_t i = 0; i < other.allCollaborations.getSize(); ++i)
	{
		allCollaborations.pushBack(new Collaboration(*other.allCollaborations[i]));
	}

	allTasks = other.allTasks;
}

void Session::moveFrom(Session&& other)
{
	currentUser = other.currentUser;
	other.currentUser = nullptr;

	allUsers = std::move(other.allUsers);
	allCollaborations = std::move(other.allCollaborations);
	allTasks = std::move(other.allTasks);
}

void Session::free()
{
	delete currentUser;

	for (size_t i = 0; i < allUsers.getSize(); ++i)
	{
		delete allUsers[i];
	}
	allUsers.clear();

	for (size_t i = 0; i < allCollaborations.getSize(); ++i)
	{
		delete allCollaborations[i];
	}
	allCollaborations.clear();
}

void Session::deleteTaskFromAllCollaborations(unsigned id)
{
	for (int i = 0; i < allCollaborations.getSize(); i++)
	{
		if (allCollaborations[i]->isTaskInCollaboration(id))
		{
			allCollaborations[i]->deleteTaskFromCollaboration(id);
			return;
		}
	}
}
