#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "NavEditState.h"
#include "Controller.h"

#include "View.h"

int main() {
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF); // This is used to auto output memory information about leaks before closing the application
	//_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG); // Set to output into your IDE's debug window

	setlocale(LC_ALL, "ru");
	pdcurses::init();
	
	View* view = new View();
	BarModel* bar = new BarModel();
	TextModel* text = new TextModel("~");

	bar->attach(view);
	text->attach(view);
	bar->setFilename("<UNTITLED>");
	bar->notify();
	text->notify();
	
	Controller* ctrl = new Controller(NavEditState::pinstance(text, bar));
	ctrl->update();

	delete ctrl;
	delete text;
	delete bar;
	delete view;

	pdcurses::end();
	return 0;
}
