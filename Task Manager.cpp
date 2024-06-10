#include <iostream>
#include "Task.h"
#include "User.h"

int main()
{
	User user1;
	Optional<MyString> date("2024-03-05");
	Optional<MyString> date2("2024-03-06");
	user1.addTask("task1", date, "test task");
	user1.addTask("task2", date, "test task");
	user1.addTask("task2", date2, "test task 3");
	user1.getTaskByID(0).setStatus(Status::OVERDUE);
	user1.addTaskToDashboard(0);
	user1.addTaskToDashboard(1);

	user1.updateTaskName(0, "new name");
	user1.startTask(1);
	user1.updateTaskDescription(1, "task VASI");
	user1.finishTask(0);
	//user1.listTask("task2");
	user1.deleteTask(2);
	//user1.listTasks(date2);
	//user1.listCompletedTasks();
	user1.listDashboard();
	//user1.removeTaskFromDashboard(0);
	//user1.listDashboard();
}
