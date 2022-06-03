#ifndef _FIND_STATE_H_
#define _FIND_STATE_H_
#include "CtrlState.h"
#include "NavEditState.h"
#include "Controller.h"

class FindState :
	public CtrlState,
	public Singleton<FindState, TextModel*, BarModel*>
{
private:
	TextModel* _txtMdl;
	BarModel* _barMdl;

public:
	FindState(guard, TextModel* txt, BarModel* bar);
	~FindState() = default;
	void update(short key) override;
};

#endif //_FIND_STATE_H_