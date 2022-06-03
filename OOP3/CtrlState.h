#ifndef _CTRL_STATE_H_
#define _CTRL_STATE_H_
#include <map>
#include "Command.h"

class Controller;

class CtrlState
{
protected:
	Controller* _ctrl;
public:
	CtrlState() = default;
	void setCtrl(Controller* ctrl) {
		_ctrl = ctrl;
	}
	virtual ~CtrlState() = default;
	virtual void update(short key) = 0;
};

template <class T>
class CmdHandler: public CtrlState {
protected:
	std::map<T, Command*> _cmds;
	Command* getCommand(T key) {
		auto it = _cmds.find(key);
		if (it == _cmds.end())
			return nullptr;
		return it->second;
	}
public:
	~CmdHandler() override {
		typename std::map<T, Command*>::iterator itr = _cmds.begin();
		for (; itr != _cmds.end(); itr++) {
			delete itr->second;
		}
	}
};

#endif //_CTRL_STATE_H_