#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

class CtrlState;

class Controller {
private:
	CtrlState* _currState = nullptr;
	bool _shouldStop = false;
public:
	Controller(CtrlState* initialState);
	~Controller() = default;
	
	void setState(CtrlState* state);
	
	void setStop();
	
	void update();
};

#endif //_CONTROLLER_H_