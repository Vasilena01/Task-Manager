#include <iostream>
//#include "Task.h"
//#include "User.h"
#include "Engine.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

int main()
{	//TODO DASHBORDA pak ne raboti... neshto s shallow copy stava sig
	//TODO deleteTask,removeTask from dashboard tasks[i] = nullptr ne mi dava...i zatova se chupi
	

	try
	{
		//Session session;
		/*User user1("test1", "pas123");
	Optional<MyString> date("2024-03-05");
	Optional<MyString> date2("2024-03-06");
	user1.addTask("task1", date, "test task");
	user1.addTask("task2", date2, "test task");
	user1.addTask("task2", date2, "test task 3");
	session.allUsers.pushBack(user1);
	session.saveUsers("users.dat");*/
		Engine engine;
		//engine.handleExit();
		engine.start();
		//Session session;
		//std::cout << session.getAllUsers()[0].getPassword();
	}
	catch (std::exception& e)
	{
		std::cout << e.what();
	}
	//engine.start();
	
	/*User user1;
	Optional<MyString> date("2024-03-05");
	Optional<MyString> date2("2024-03-06");
	user1.addTask("task1", date, "test task");
	user1.addTask("task2", date, "test task");
	user1.addTask("task2", date2, "test task 3");
	user1.listTasks();
	user1.deleteTask(0);*/
	//user1.getTaskByID(0)->setStatus(Status::OVERDUE);
	//user1.addTaskToDashboard(0);
	//user1.addTaskToDashboard(2);
	//user1.listDashboard();
	////user1.removeTaskFromDashboard(0);
	//user1.listDashboard();

	//user1.updateTaskName(0, "new name");
	//user1.listTasks();
	//user1.startTask(1);
	//user1.updateTaskDescription(1, "task VASI");
	//user1.finishTask(0);
	//user1.listTasks();
	//user1.listTask("task2");
	//user1.deleteTask(1);
	//user1.listTasks();
	//user1.listTask("task2");
	//user1.listTasks(date2);
	//user1.listCompletedTasks();
}
