#include "FindState.h"
FindState::FindState(guard, TextModel* txt, BarModel* bar) : _txtMdl(txt), _barMdl(bar) {
	bar->setState('F');
	bar->setFind("");
}

void FindState::update(short key) {
	if (isprint(static_cast<char>(key)))
		_barMdl->appendDotsCmd(static_cast<char>(key));

	if (key == 10) {
		MyString to_find = _barMdl->getDotsCmd().substr(1);
		_barMdl->setFind(to_find);
		if (_barMdl->getDotsCmd()[0] == '?') {
			FindToBegCommand cmd(*_txtMdl);
			cmd.setText(_barMdl->getLastSearch());
			cmd.execute();
		}
		else if (_barMdl->getDotsCmd()[0] == '/') {
			FindToEndCommand cmd(*_txtMdl);
			cmd.setText(_barMdl->getLastSearch());
			cmd.execute();
		}
	}

	if (key == 27 || key == 10) {
		_barMdl->setDotsCmd("");
		_barMdl->setState('N');
		_ctrl->setState(NavEditState::pinstance(_txtMdl, _barMdl));
	}
}
