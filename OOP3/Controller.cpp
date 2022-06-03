#include "Controller.h"
#include "CtrlState.h"

Controller::Controller(CtrlState* initialState) {
	setState(initialState);
}

void Controller::setState(CtrlState* state) {
	_currState = state;
	_currState->setCtrl(this);
}

void Controller::setStop() {
	_shouldStop = true;
}

void Controller::update() {
	while (!_shouldStop) {
		int key = pdcurses::scr::getch();
		if (_currState == nullptr)
			break;
		_currState->update(static_cast<short>(key));
	}
}
