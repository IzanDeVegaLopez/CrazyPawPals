// This file is part of the course TPV2@UCM - Samir Genaim
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include<crtdbg.h>
#include <iostream>

#include "game/Game.h"

int main(int, char**) {

	//para buscar donde esta el memory leaks
	//_CrtSetBreakAlloc(165116); 
	try {
		if(Game::Init())Game::Instance()->start();
	}
	catch (const std::string& e) { // catch exceptions thrown as strings
		std::cerr << e << std::endl;
	}
	catch (const char* e) { // catch exceptions thrown as char*
		std::cerr << e << std::endl;
	}
	catch (const std::exception& e) { // catch exceptions thrown as a sub-type of std::exception
		std::cerr << e.what();
	}
	catch (...) {
		std::cerr << "Caught and exception of unknown type ...";
	}

	_CrtDumpMemoryLeaks();
	return 0;
}