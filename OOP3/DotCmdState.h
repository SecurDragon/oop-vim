#ifndef _DOTCMD_STATE_H_
#define _DOTCMD_STATE_H_

#include "BarModel.h"
#include "CtrlState.h"
#include "Controller.h"
#include "NavEditState.h"

class DotCmdState:
	public CmdHandler<MyStringDecorator>,
	public Singleton<DotCmdState, TextModel*, BarModel*>
{
private:
	TextModel* _txtMdl;
	BarModel* _barMdl;
protected:
	Command* getCommand(MyString& str);
public:
	DotCmdState(guard, TextModel* text, BarModel* bar);
	void update(short key) override;
	~DotCmdState() = default;
};

#endif //_DOTCMD_STATE_H_