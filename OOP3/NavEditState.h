#ifndef _NAVEDIT_STATE_H_
#define _NAVEDIT_STATE_H_

#include "BarModel.h"
#include "CtrlState.h"
#include "Command.h"
#include "History.h"

class NavEditState :
	public CmdHandler<MyStringDecorator>,
	public Singleton<NavEditState, TextModel*, BarModel*>
{
private:
	std::map<short, Command*> _navcmds;
	TextModel* _txtMdl;
	BarModel* _barMdl;
	bool checkSubstr(MyStringDecorator const& cmd);
public:
	NavEditState(guard, TextModel* text, BarModel* bar);
	void update(short key) override;
	~NavEditState();
};

#endif //_NAVEDIT_STATE_H_