#ifndef _TEXTEDIT_STATE_H_
#define _TEXTEDIT_STATE_H_

#include "CtrlState.h"
#include "NavEditState.h"

class TextEditState :
	public CmdHandler<short>,
	public Singleton<TextEditState, TextModel*, BarModel*>
{
private:
	TextModel* _txtMdl;
	BarModel* _barMdl;
public:
	TextEditState(guard, TextModel* mdl, BarModel* bar);
	~TextEditState() = default;
	void update(short key) override;
};

#endif //_TEXTEDIT_STATE_H_
