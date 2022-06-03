#include "TextEditState.h"
#include "Controller.h"

TextEditState::TextEditState(guard, TextModel* mdl, BarModel* bar)
	: _txtMdl(mdl), _barMdl(bar) {
	bar->setState('I');

	_cmds.insert(std::make_pair(static_cast<short>('\n'), new PrintCommand(*mdl, '\n')));
	_cmds.insert(std::make_pair(static_cast<short>('\b'), new BackspaceEraseCommand(*mdl)));
	_cmds.insert(std::make_pair(static_cast<short>(KEY_DC), new DeleteEraseCommand(*mdl)));
	_cmds.insert(std::make_pair(static_cast<short>(KEY_RIGHT), new MoveCursorCommand(*mdl, MoveCursorCommand::CursorDirection::RIGHT)));
	_cmds.insert(std::make_pair(static_cast<short>(KEY_UP), new MoveCursorCommand(*mdl, MoveCursorCommand::CursorDirection::UP)));
	_cmds.insert(std::make_pair(static_cast<short>(KEY_LEFT), new MoveCursorCommand(*mdl, MoveCursorCommand::CursorDirection::LEFT)));
	_cmds.insert(std::make_pair(static_cast<short>(KEY_DOWN), new MoveCursorCommand(*mdl, MoveCursorCommand::CursorDirection::DOWN)));
	_cmds.insert(std::make_pair(static_cast<short>(KEY_PPAGE), new MoveCursorCommand(*mdl, MoveCursorCommand::CursorDirection::SCR_UP, 29)));
	_cmds.insert(std::make_pair(static_cast<short>(KEY_NPAGE), new MoveCursorCommand(*mdl, MoveCursorCommand::CursorDirection::SCR_DOWN, 29)));

	_cmds.insert(std::make_pair(static_cast<short>(27), new LambdaCommand([&]() {
		_barMdl->setState('N');
		this->_ctrl->setState(NavEditState::pinstance(_txtMdl, _barMdl));
	})));
}

void TextEditState::update(short key) {
	if (key >= 0 && key <= 255 && isprint(key)) {
		_txtMdl->append(static_cast<char>(key));
		_barMdl->setModified();
	}
	else {
		Command* cmd = getCommand(key);
		if (cmd != nullptr)
			_barMdl->setLastError("");
		cmd->execute();
	}
}
