#include <iostream>
#include "Engine.h"

// NOTE: because of the check for eof in loadUsers and loadCollaborations functions in Session.cpp
// it is necessary both users.dat and collaborations.dat files to have some information in them 
// before running the program, otherwise it will throw bad allocation error
int main()
{
	try
	{
		Engine engine;
		engine.start();
	}
	catch (std::exception& e)
	{
		std::cout << e.what();
	}
}
