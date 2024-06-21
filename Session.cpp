#include "Session.h"
#include "fstream";

Session::Session()
{
	loadUsers("users.dat");
	/*loadTasks("tasks.dat");
	loadCollaborations("collaborations.dat");*/
}

Session::~Session()
{
	exit();
}

void Session::registerUser(const MyString& username, const MyString& password)
{
	for (size_t i = 0; i < allUsers.getSize(); i++)
	{
		if (allUsers[i].getUsername() == username)
		{
			std::cerr << "User already exists." << std::endl;
			return;
			//throw std::logic_error("User already exists.");
		}
	}

	User newUser(username, password);
	allUsers.pushBack(newUser);

	std::cout << "Registered successfully!" << std::endl;
}

bool Session::login(const MyString& username, const MyString& password)
{
	for (size_t i = 0; i < allUsers.getSize(); i++)
	{
		if (allUsers[i].getUsername() == username && allUsers[i].getPassword() == password)
		{
			currentUser = UniquePtr<User>(new User(allUsers[i]));
			std::cout << "Welcome back, " << username << "!" << std::endl;
			return true;
		}
	}
	std::cerr << "Invalid username or password." << std::endl;
	return false;
}

void Session::logout()
{
	//Updating the allUsers collection when loging out
	if (currentUser)
	{
		int index = findUserIndexByUsername(currentUser->getUsername());
		if (index != -1)
		{
			allUsers[index] = *currentUser;
		}
		else {
			std::cerr << "User not found in the user list!" << std::endl;
		}

		std::cout << "Logged out: " << currentUser->getUsername() << std::endl;
		currentUser = nullptr;
	}
}

void Session::exit()
{

	if (currentUser)
	{
		int index = findUserIndexByUsername(currentUser->getUsername());
		if (index != -1)
		{
			allUsers[index] = *currentUser;
		}
	}

	try
	{
		saveUsers("users.dat");
		//TODO - remove listTasks()
		
		//currentUser->listTasks();
	}
	catch (std::exception& e)
	{
		std::cout << e.what();
	}
	
	//saveTasks("tasks.dat");
	//saveCollaborations("collaborations.dat");
	std::cout << "Session ended. All data saved." << std::endl;
}

User* Session::getCurrentUser() const
{
	return currentUser.getPtr();
}

Vector<User> Session::getAllUsers() const
{
	return allUsers;
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
		User user(newUsername, newPassword);
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
			size_t descriptionLength = 0;
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
			user.addTask(task);

			// Checks if the task's due_date is today and adds it to the dashboard
			if (hasDueDate) {
				std::tm taskDate = task->getDate().getValue();
				if (areDatesEqual(taskDate, today)) {
					user.addTaskToDashboard(task->getId());
				}
			}

			delete[] taskName;
			delete[] description;
			delete task;
		}

		// Read number of dashboard tasks
		size_t dashboardTaskCount;
		file.read((char*)(&dashboardTaskCount), sizeof(dashboardTaskCount));

		// Read and add dashboard tasks
		for (size_t i = 0; i < dashboardTaskCount; i++) {
			unsigned dashboardTaskId;
			file.read((char*)(&dashboardTaskId), sizeof(dashboardTaskId));
			user.addTaskToDashboard(dashboardTaskId);
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
		const MyString& username = allUsers[i].getUsername();
		size_t usernameSize = username.getSize();
		file.write((const char*)&usernameSize, sizeof(usernameSize));
		file.write(username.c_str(), usernameSize);

		// Write password
		const MyString& password = allUsers[i].getPassword();
		size_t passwordSize = password.getSize();
		file.write((const char*)&passwordSize, sizeof(passwordSize));
		file.write(password.c_str(), passwordSize);

		// Write number of tasks
		size_t taskCount = allUsers[i].getTasks().getSize();
		file.write((const char*)(&taskCount), sizeof(taskCount));

		TasksCollection currUserTasks = allUsers[i].getTasks();
		for (size_t j = 0; j < taskCount; j++)
		{
			Task* task = currUserTasks[j];
			task->printTask();

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
		size_t dashboardTaskCount = allUsers[i].getDashboard().getTasks().getSize();
		file.write((const char*)(&dashboardTaskCount), sizeof(dashboardTaskCount));

		// Write dashboard task IDs
		for (size_t j = 0; j < dashboardTaskCount; j++) {
			const Task* dashboardTask = currUserTasks[j];

			unsigned dashboardTaskId = dashboardTask->getId();
			file.write((const char*)(&dashboardTaskId), sizeof(dashboardTaskId));
		}
	}
}

void Session::saveTasks(std::ofstream& file, TasksCollection& currUserTasks, size_t taskCount)
{
	for (size_t j = 0; j < taskCount; j++)
	{
		Task* task = currUserTasks[j];
		task->printTask();

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
		/*else
		{
			due_date = MyString("");
			hasDueDate = 0;
			file.write((const char*)(&hasDueDate), sizeof(hasDueDate));
		}*/

		// Write task status
		Status taskStatus = task->getStatus();
		file.write((const char*)(&taskStatus), sizeof(taskStatus));

		// Write task description
		const MyString& description = task->getDescription();
		size_t descriptionLength = description.getSize();
		file.write((const char*)(&descriptionLength), sizeof(descriptionLength));
		file.write(description.c_str(), descriptionLength);
	}
}

//
//void Session::loadCollaborations(const MyString& fileName)
//{
//	std::ifstream file(fileName.c_str(), std::ios::binary | std::ios::in);
//	if (!file.is_open())
//		throw std::runtime_error("Failed to open file for reading!");
//
//	while (!file.eof())
//	{
//		/*Collaboration collaboration;
//		file.read((char*)(&collaboration), sizeof(Task));
//		allCollaborations.pushBack(collaboration);*/
//	}
//}
//
//void Session::saveCollaborations(const MyString& fileName) const
//{
//	std::ofstream file(fileName.c_str(), std::ios::binary | std::ios::out);
//	if (!file.is_open())
//		throw std::runtime_error("Failed to open file for writing!");
//
//	/*for (size_t i = 0; i < allCollaborations.getSize(); i++)
//	{
//		Collaboration collab;
//		file.write((const char*)(&collab), sizeof(Collaboration));
//	}*/
//}

// TODO IS THERE A BETTER WAY TO GET THE CURRENT LOCAL TIME???
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

int Session::findUserIndexByUsername(const MyString& username) {
	for (size_t i = 0; i < allUsers.getSize(); ++i) {
		if (allUsers[i].getUsername() == username) {
			return i;
		}
	}
	return -1; // User not found
}