#include "Engine.h"
#include <sstream>

void Engine::start()
{
	char command[GlobalConstants::BUFF_SIZE];
	std::cout << "Welcome to your task manager!" << std::endl;
	std::cout << "Choose: register / login / exit" << std::endl;

	bool isRunning = true;
	while (isRunning)
	{
		std::cout << "> ";
		std::cin.getline(command, sizeof(command));
		MyString commandStr(command);

		try {
			if (command == "exit")
			{
				isRunning = false;
				handleExit();
				break;
			}
			else
			{
				handleCommand(commandStr, isRunning);
			}
		}
		catch (const std::invalid_argument& e)
		{
			std::cerr << "Invalid argument error: " << e.what() << std::endl;
		}
		catch (const std::out_of_range& e)
		{
			std::cerr << "Out of range error: " << e.what() << std::endl;
		}
		catch (const std::logic_error& e)
		{
			std::cerr << "Logic error: " << e.what() << std::endl;
		}
		catch (const std::runtime_error& e)
		{
			std::cerr << "Runtime error: " << e.what() << std::endl;
		}
		catch (const std::exception& e)
		{
			std::cerr << "An error occurred: " << e.what() << std::endl;
		}
		catch (...)
		{
			std::cerr << "An unknown error occurred." << std::endl;
		}
	}
}

void Engine::handleCommand(const MyString& command, bool& isRunning)
{
	// Converting the command from MyString to be readable in C-style and to be able to compare it bellow
	std::stringstream ss(command.c_str());
	char buff[GlobalConstants::BUFF_SIZE];
	ss >> buff;

	if (strcmp(buff, "register") == 0)
	{
		char username[GlobalConstants::BUFF_SIZE], password[GlobalConstants::BUFF_SIZE];
		ss >> username >> password;
		handleRegister(username, password);
	}
	else if (strcmp(buff, "login") == 0)
	{
		char username[GlobalConstants::BUFF_SIZE], password[GlobalConstants::BUFF_SIZE];
		ss >> username >> password;
		handleLogin(username, password);
	}
	else if (strcmp(buff, "logout") == 0)
	{
		handleLogout();
	}
	else if (strcmp(buff, "add-task") == 0)
	{
		char name[GlobalConstants::BUFF_SIZE];
		char due_date_str[GlobalConstants::BUFF_SIZE];
		char description[GlobalConstants::BUFF_SIZE];
		ss >> name;

		ss >> due_date_str;

		if (isValidDate(due_date_str))
		{
			std::tm due_date = {};
			try {
				due_date = parseDate(due_date_str);
			}
			catch (const std::exception& e) {
				std::cerr << "Error: " << e.what() << std::endl;
				return;
			}
			ss.getline(description, sizeof(description));
			handleAddTask(name, due_date, description);
		}
		else
		{
			ss.clear();
			ss.seekg(std::ios::beg); // Rewind to read everything after the name
			ss.ignore(std::numeric_limits<std::streamsize>::max(), ' '); // Skip the command part
			ss.getline(description, sizeof(description));
			handleAddTask(name, description);
		}
	}
	else if (strcmp(buff, "update-task-name") == 0)
	{
		unsigned id;
		char name[GlobalConstants::BUFF_SIZE];
		ss >> id;
		ss.ignore();
		ss.getline(name, sizeof(name));
		handleUpdateTaskName(id, name);
	}
	else if (strcmp(buff, "start-task") == 0)
	{
		unsigned id;
		ss >> id;
		handleStartTask(id);
	}
	else if (strcmp(buff, "update-task-description") == 0)
	{
		unsigned id;
		char description[GlobalConstants::BUFF_SIZE];
		ss >> id;
		ss.ignore();
		ss.getline(description, sizeof(description));
		handleUpdateTaskDescription(id, description);
	}
	else if (strcmp(buff, "remove-task-from-dashboard") == 0)
	{
		unsigned id;
		ss >> id;
		handleRemoveTaskFromDashboard(id);
	}
	else if (strcmp(buff, "add-task-to-dashboard") == 0)
	{
		unsigned id;
		ss >> id;
		handleAddTaskToDashboard(id);
	}
	else if (strcmp(buff, "delete-task") == 0)
	{
		unsigned id;
		ss >> id;
		handleDeleteTask(id);
	}
	else if (strcmp(buff, "get-task") == 0)
	{
		User* currentUser = session.getCurrentUser();
		if (currentUser)
		{
			try
			{
				unsigned taskId = 0;
				ss >> taskId;
				if (ss.fail())
					throw std::runtime_error("Invalid id!");

				currentUser->listTask(taskId);
			}
			catch (const std::exception& e)
			{
				ss.clear();
				size_t currentPos = ss.tellg();
				ss.seekg(0, std::ios::end);
				size_t endPos = ss.tellg();
				size_t remainingSymbolsCount = endPos - currentPos;
				ss.seekg(currentPos);

				char* remainder = new char[remainingSymbolsCount];
				ss.read(remainder, remainingSymbolsCount);

				MyString name(remainder);
				name = name.substr(0, remainingSymbolsCount);

				currentUser->listTask(name);
			}
		}
		else
		{
			std::cerr << "No user logged in." << std::endl;
		}
	}
	else if (strcmp(buff, "list-tasks") == 0)
	{
		char due_date_str[GlobalConstants::BUFF_SIZE];
		ss >> due_date_str;

		if (isValidDate(due_date_str))
		{
			std::tm due_date = {};

			due_date = parseDate(due_date_str);
			handleListTasks(due_date);
		}
		else
		{
			handleListTasks();
		}
	}
	else if (strcmp(buff, "list-completed-tasks") == 0)
	{
		handleListCompletedTasks();
	}
	else if (strcmp(buff, "list-dashboard") == 0)
	{
		handleListDashboard();
	}
	else if (strcmp(buff, "finish-task") == 0)
	{
		unsigned id;
		ss >> id;
		handleFinishTask(id);
	}
	else if (strcmp(buff, "add-collaboration") == 0)
	{
		char name[GlobalConstants::BUFF_SIZE];
		ss >> name;
		handleAddCollaboration(name);
	}
	else if (strcmp(buff, "delete-collaboration") == 0)
	{
		char name[GlobalConstants::BUFF_SIZE];
		ss >> name;
		handleDeleteCollaboration(name);
	}
	else if (strcmp(buff, "list-collaborations") == 0)
	{
		handleListCollaborations();
	}
	else if (strcmp(buff, "add-user") == 0)
	{
		char collabName[GlobalConstants::BUFF_SIZE];
		char username[GlobalConstants::BUFF_SIZE];

		ss >> collabName >> username;
		handleAddUserToCollaboration(collabName, username);
	}
	else if (strcmp(buff, "assign-task") == 0)
	{
		char collabName[GlobalConstants::BUFF_SIZE];
		char username[GlobalConstants::BUFF_SIZE];
		char name[GlobalConstants::BUFF_SIZE];
		char due_date_str[GlobalConstants::BUFF_SIZE];
		char description[GlobalConstants::BUFF_SIZE];

		ss >> collabName;
		ss >> username;
		ss >> name;

		ss >> due_date_str;
		std::tm due_date = {};
		try {
			due_date = parseDate(due_date_str);
		}
		catch (const std::exception& e) {
			std::cerr << "Error: " << e.what() << std::endl;
			return;
		}
		ss.getline(description, sizeof(description));

		handleAssignTask(collabName, username, name, due_date, description);
	}
	else if (strcmp(buff, "list-collab-tasks") == 0)
	{
		char collabName[GlobalConstants::BUFF_SIZE];

		ss >> collabName;
		handleListCollaborationTasks(collabName);
	}
	else if (strcmp(buff, "exit") == 0)
	{
		isRunning = false;
		handleExit();
		return;
	}
	else
	{
		std::cout << "Invalid command!" << std::endl;
	}
}

void Engine::handleRegister(const MyString& username, const MyString& password)
{
	session.registerUser(username, password);
}

bool Engine::handleLogin(const MyString& username, const MyString& password)
{
	if (session.login(username, password))
		return true;
	return false;
}

void Engine::handleLogout()
{
	session.logout();
	std::cout << "Logged out." << std::endl;
}

void Engine::handleExit()
{
	session.exit();
	std::cout << "Exiting program." << std::endl;
}

void Engine::handleAddTask(const MyString& name, const std::tm& due_date, const MyString& description)
{
	try
	{
		User* currentUser = session.getCurrentUser();
		if (!currentUser)
		{
			throw std::runtime_error("No user logged in.");
		}

		if (name.getSize() == 0)
		{
			throw std::invalid_argument("Task name cannot be empty.");
		}

		currentUser->addTask(name, due_date, description);

		std::cout << "Task added successfully!" << std::endl;
	}
	catch (const std::logic_error& e)
	{
		std::cerr << "Logic error while adding task: " << e.what() << std::endl;
	}
	catch (const std::runtime_error& e)
	{
		std::cerr << "Runtime error while adding task: " << e.what() << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cerr << "An error occurred while adding task: " << e.what() << std::endl;
	}
}

void Engine::handleAddTask(const MyString& name, const MyString& description)
{
	try
	{
		User* currentUser = session.getCurrentUser();
		if (!currentUser)
		{
			throw std::runtime_error("No user logged in.");
		}

		if (name.getSize() == 0)
		{
			throw std::invalid_argument("Task name cannot be empty.");
		}

		currentUser->addTask(name, description);

		std::cout << "Task added successfully!" << std::endl;
	}
	catch (const std::logic_error& e)
	{
		std::cerr << "Logic error while adding task: " << e.what() << std::endl;
	}
	catch (const std::runtime_error& e)
	{
		std::cerr << "Runtime error while adding task: " << e.what() << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cerr << "An error occurred while adding task: " << e.what() << std::endl;
	}
}

void Engine::handleUpdateTaskName(unsigned id, const MyString& name)
{
	try
	{
		User* currentUser= session.getCurrentUser();
		if (currentUser)
		{
			currentUser->updateTaskName(id, name);
			std::cout << "Name set successfully!" << std::endl;
		}
		else
		{
			std::cerr << "No user logged in." << std::endl;
		}
	}
	catch (const std::logic_error& e)
	{
		std::cerr << e.what() << std::endl;
	}
	catch (const std::runtime_error& e)
	{
		std::cerr << e.what() << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
}

void Engine::handleStartTask(unsigned id)
{
	User* currentUser = session.getCurrentUser();
	if (currentUser)
	{
		currentUser->startTask(id);
		std::cout << "Task started successfully!" << std::endl;
	}
	else
	{
		std::cerr << "No user logged in." << std::endl;
	}
}

void Engine::handleUpdateTaskDescription(unsigned id, const MyString& description)
{
	User* currentUser = session.getCurrentUser();
	if (currentUser)
	{
		currentUser->updateTaskDescription(id, description);
		std::cout << "Task description updated successfully!" << std::endl;
	}
	else
	{
		std::cerr << "No user logged in." << std::endl;
	}
}

void Engine::handleRemoveTaskFromDashboard(unsigned id)
{
	User* currentUser = session.getCurrentUser();
	if (currentUser)
	{
		currentUser->removeTaskFromDashboard(id);
		std::cout << "Task removed from dashboard successfully!" << std::endl;
	}
	else
	{
		std::cerr << "No user logged in." << std::endl;
	}
}

void Engine::handleAddTaskToDashboard(unsigned id)
{
	User* currentUser = session.getCurrentUser();
	if (currentUser)
	{
		currentUser->addTaskToDashboard(id);
		std::cout << "Task added to dashboard successfully!" << std::endl;
	}
	else
	{
		std::cerr << "No user logged in." << std::endl;
	}
}

void Engine::handleDeleteTask(unsigned id)
{
	User* currentUser = session.getCurrentUser();
	if (currentUser)
	{
		currentUser->deleteTask(id);
		session.deleteTaskFromAllCollaborations(id);
		std::cout << "Task deleted successfully!" << std::endl;
	}
	else
	{
		std::cerr << "No user logged in." << std::endl;
	}
}

void Engine::handleListTasks(const std::tm& due_date)
{
	User* currentUser = session.getCurrentUser();
	if (currentUser)
	{
		currentUser->listTasks(due_date);
	}
	else
	{
		std::cerr << "No user logged in." << std::endl;
	}
}

void Engine::handleListTasks()
{
	User* currentUser = session.getCurrentUser();
	if (currentUser)
	{
		currentUser->listTasks();
	}
	else
	{
		std::cerr << "No user logged in." << std::endl;
	}
}

void Engine::handleListCompletedTasks()
{
	User* currentUser = session.getCurrentUser();
	if (currentUser)
	{
		currentUser->listCompletedTasks();
	}
	else
	{
		std::cerr << "No user logged in." << std::endl;
	}
}

void Engine::handleListDashboard()
{
	User* currentUser = session.getCurrentUser();
	if (currentUser)
	{
		currentUser->listDashboard();
	}
	else
	{
		std::cerr << "No user logged in." << std::endl;
	}
}

void Engine::handleFinishTask(unsigned id)
{
	User* currentUser = session.getCurrentUser();
	if (currentUser)
	{
		currentUser->finishTask(id);
	}
	else
	{
		std::cerr << "No user logged in." << std::endl;
	}
}

void Engine::handleAddCollaboration(const MyString& name)
{
	session.addCollaboration(name);
}

void Engine::handleDeleteCollaboration(const MyString& name)
{
	session.deleteCollaboration(name);
	std::cout << "Collaboration deleted successfully!" << std::endl;
}

void Engine::handleListCollaborations()
{
	session.listCollaborations();
}

void Engine::handleAddUserToCollaboration(const MyString& collabName, const MyString& username)
{
	session.addUserToCollaboration(collabName, username);
	std::cout << "User added successfully!" << std::endl;
}

void Engine::handleAssignTask(const MyString& collabName, const MyString& username, 
	const MyString& taskName, const std::tm& dueDate, const MyString& description)
{
	session.assignTaskInCollaboration(collabName, username, taskName, dueDate, description);
	std::cout << "Task assigned successfully!" << std::endl;
}

void Engine::handleListCollaborationTasks(const MyString& collabName)
{
	session.listCollabTasks(collabName);
}

bool Engine::isDateFilled(const std::tm& date)
{
	return (date.tm_year != 0 || date.tm_mon != 0 || date.tm_mday != 0);
}

bool Engine::isValidDate(const char* dateStr)
{
	try {
		parseDate(dateStr);
		return true;
	}
	catch (...) {
		return false;
	}
}

std::tm Engine::parseDate(const char* dateStr)
{
	std::tm tm = {};
	if (sscanf_s(dateStr, "%4d-%2d-%2d", &tm.tm_year, &tm.tm_mon, &tm.tm_mday) != 3) {
		throw std::runtime_error("Failed to parse date: " + std::string(dateStr));
	}

	tm.tm_year -= 1900;
	tm.tm_mon -= 1;
	tm.tm_hour = 0;
	tm.tm_min = 0;
	tm.tm_sec = 0;
	tm.tm_isdst = -1;
	return tm;
}


