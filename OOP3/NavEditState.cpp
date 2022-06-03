#include "NavEditState.h"

#include "TextEditState.h"
#include "Controller.h"
#include "DotCmdState.h"
#include "FindState.h"
#include "History.h"

bool NavEditState::checkSubstr(MyStringDecorator const& cmd) {
	for(auto const& kv : _cmds) {
		auto& cmdText = const_cast<MyStringDecorator&>(kv.first);
		try {
			size_t idx = cmdText.find(cmd.c_str());
			if (idx == 0)
				return true;
		}
		catch (...){}
	}
	return false;
}

NavEditState::NavEditState(guard, TextModel* text, BarModel* bar): _txtMdl(text), _barMdl(bar) {
	_barMdl->setState('N');
	_navcmds.insert(std::make_pair(static_cast<short>(KEY_RIGHT), new MoveCursorCommand(*_txtMdl, MoveCursorCommand::CursorDirection::RIGHT)));
	_navcmds.insert(std::make_pair(static_cast<short>(KEY_UP), new MoveCursorCommand(*_txtMdl, MoveCursorCommand::CursorDirection::UP)));
	_navcmds.insert(std::make_pair(static_cast<short>(KEY_LEFT), new MoveCursorCommand(*_txtMdl, MoveCursorCommand::CursorDirection::LEFT)));
	_navcmds.insert(std::make_pair(static_cast<short>(KEY_DOWN), new MoveCursorCommand(*_txtMdl, MoveCursorCommand::CursorDirection::DOWN)));
	_navcmds.insert(std::make_pair(static_cast<short>(KEY_PPAGE), new MoveCursorCommand(*_txtMdl, MoveCursorCommand::CursorDirection::SCR_UP, 29)));
	_navcmds.insert(std::make_pair(static_cast<short>(KEY_NPAGE), new MoveCursorCommand(*_txtMdl, MoveCursorCommand::CursorDirection::SCR_DOWN, 29)));
	
	_cmds.insert(std::make_pair("^", new GotoLineBegCommand(*_txtMdl)));
	_cmds.insert(std::make_pair("0", new GotoLineBegCommand(*_txtMdl)));
	_cmds.insert(std::make_pair("$", new GotoLineEndCommand(*_txtMdl)));
	_cmds.insert(std::make_pair("gg", new GotoTextBegCommand(*_txtMdl)));
	_cmds.insert(std::make_pair("G", new GotoTextEndCommand(*_txtMdl)));
	_cmds.insert(std::make_pair("x", new LambdaCommand([&]() {
		History::instance().add(_txtMdl->save(), _txtMdl->getCurrentLineNo());
		DeleteEraseCommand(*_txtMdl).execute();
	})));
	_cmds.insert(std::make_pair("X", new LambdaCommand([&]() {
		History::instance().add(_txtMdl->save(), _txtMdl->getCurrentLineNo());
		BackspaceEraseCommand(*_txtMdl).execute();
	})));
	_cmds.insert(std::make_pair("w", new GotoNextWordCommand(*_txtMdl)));
	_cmds.insert(std::make_pair("W", new GotoNextWordCommand(*_txtMdl, true)));
	_cmds.insert(std::make_pair("b", new GotoPrevWordCommand(*_txtMdl)));
	_cmds.insert(std::make_pair("B", new GotoPrevWordCommand(*_txtMdl, true)));
	_cmds.insert(std::make_pair("dw", new DeleteToWEndCommand(*_txtMdl, true)));
	_cmds.insert(std::make_pair("dE", new DeleteToWEndCommand(*_txtMdl)));
	_cmds.insert(std::make_pair("diw", new DeleteWordCommand(*_txtMdl)));
	_cmds.insert(std::make_pair("J", new JunctionCommand(*_txtMdl)));
	_cmds.insert(std::make_pair("d^", new DelToLineBegCommand(*_txtMdl)));
	_cmds.insert(std::make_pair("d0", new DelToLineBegCommand(*_txtMdl)));
	_cmds.insert(std::make_pair("d$", new DelToLineEndCommand(*_txtMdl)));
	_cmds.insert(std::make_pair("D", new DelToLineEndCommand(*_txtMdl)));
	_cmds.insert(std::make_pair("p", new PutCBAfterCommand(*_txtMdl)));
	_cmds.insert(std::make_pair("P", new PutCBBeforeCommand(*_txtMdl)));
	_cmds.insert(std::make_pair("dd", new CutCurrLineCommand(*_txtMdl)));
	_cmds.insert(std::make_pair("Y", new CopyCurrLineCommand(*_txtMdl)));
	_cmds.insert(std::make_pair("yw", new CopyWordCommand(*_txtMdl)));
	_cmds.insert(std::make_pair("y^", new CopyToLineBegCommand(*_txtMdl)));
	_cmds.insert(std::make_pair("y0", new CopyToLineBegCommand(*_txtMdl)));
	_cmds.insert(std::make_pair("y$", new CopyToLineEndCommand(*_txtMdl)));
	_cmds.insert(std::make_pair("e!", new LambdaCommand([&]() {
		History::instance().undoAll();
	})));


	_cmds.insert(std::make_pair("r", new LambdaCommand([&]() {
		_barMdl->setState('I');
		int ch = pdcurses::scr::getch();
		if (isprint(ch)) {
			_txtMdl->erase(1);
			_txtMdl->append(static_cast<char>(ch));
		}
		_barMdl->setState('N');
	})));
	_cmds.insert(std::make_pair("i", new LambdaCommand([&]() {
		_barMdl->setState('I');		
		_ctrl->setState(TextEditState::pinstance(_txtMdl, _barMdl));
	})));
	_cmds.insert(std::make_pair(":", new LambdaCommand([&]() {
		_barMdl->setDotsCmd(":");
		_barMdl->setState('C');
		_ctrl ->setState(DotCmdState::pinstance(_txtMdl, _barMdl)); })));
	_cmds.insert(std::make_pair("/", new LambdaCommand([&]() {
		_barMdl->setDotsCmd("/");
		_barMdl->setState('F');
		_ctrl ->setState(FindState::pinstance(_txtMdl, _barMdl)); })));
	_cmds.insert(std::make_pair("?", new LambdaCommand([&]() {
		_barMdl->setDotsCmd("?");
		_barMdl->setState('F');
		_ctrl ->setState(FindState::pinstance(_txtMdl, _barMdl));
	})));

	_cmds.insert(std::make_pair("n", new LambdaCommand([&]() {
		FindToEndCommand cmd(*_txtMdl);
		cmd.setText(_barMdl->getLastSearch());
		cmd.execute();
	})));
	_cmds.insert(std::make_pair("N", new LambdaCommand([&]() {
		FindToBegCommand cmd(*_txtMdl);
		cmd.setText(_barMdl->getLastSearch());
		cmd.execute();
	})));

	
	_cmds.insert(std::make_pair("a", new LambdaCommand([&]() {
		_txtMdl->setIndex(_txtMdl->getIndex() + 1);
		_barMdl->setState('I');
		_ctrl->setState(TextEditState::pinstance(_txtMdl, _barMdl));
	})));
	_cmds.insert(std::make_pair("I", new LambdaCommand([&]() {
		auto& text = _txtMdl->getText();
		size_t idx = text.rfind("\n", _txtMdl->getIndex());
		if (idx == MyString::npos)
			idx = 0;
		_txtMdl->setIndex(idx);
		_barMdl->setState('I');
		_ctrl->setState(TextEditState::pinstance(_txtMdl, _barMdl));
	})));
	
	_cmds.insert(std::make_pair("A", new LambdaCommand([&]() {
		auto& text = _txtMdl->getText();
		size_t idx = text.find("\n", _txtMdl->getIndex());
		if (idx == MyString::npos)
			idx = text.size() - 1;
		_txtMdl->setIndex(idx);
		_barMdl->setState('I');
		_ctrl->setState(TextEditState::pinstance(_txtMdl, _barMdl));
	})));

	_cmds.insert(std::make_pair("o", new LambdaCommand([&]() {
		auto& text = _txtMdl->getText();
		size_t idx = text.rfind("\n", _txtMdl->getIndex());
		if (idx == MyString::npos)
			idx = 0;
		text.insert(idx, 1, '\n');
		_txtMdl->setIndex(idx);
		_barMdl->setState('I');
		_ctrl->setState(TextEditState::pinstance(_txtMdl, _barMdl));
	})));

	_cmds.insert(std::make_pair("O", new LambdaCommand([&]() {
		auto& text = _txtMdl->getText();
		size_t idx = text.find("\n", _txtMdl->getIndex());
		if (idx == MyString::npos)
			idx = text.size();
		text.insert(idx, 1, '\n');
		_txtMdl->setIndex(idx + 1);
		_barMdl->setState('I');
		_ctrl->setState(TextEditState::pinstance(_txtMdl, _barMdl));
	})));

	_cmds.insert(std::make_pair("S", new LambdaCommand([&]() {
		Command* del = new EraseCurrLineCommand(*_txtMdl);
		del->execute();
		delete del;
		_barMdl->setState('I');
		_ctrl->setState(TextEditState::pinstance(_txtMdl, _barMdl));
	})));

	_cmds.insert(std::make_pair("u", new LambdaCommand([&]() {
		History::instance().undo();
	})));

	_cmds.insert(std::make_pair("U", new LambdaCommand([&]() {
		History::instance().undoLine(_txtMdl->getCurrentLineNo());
	})));
	
}

void NavEditState::update(short key) {
	auto it = _navcmds.find(key);
	if (it != _navcmds.end()) {
		it->second->execute();
		return;
	}

	if (key == ERR || !isprint(static_cast<char>(key))) return;

	_barMdl->appendBtnCmd(static_cast<char>(key));
	Command* cmd = getCommand(_barMdl->getBtnCmd());
	if (cmd != nullptr) {
		_barMdl->eraseBtnCmd();
		_barMdl->setLastError("");
		cmd->execute();
	}
	else if (!checkSubstr(_barMdl->getBtnCmd()))
		_barMdl->eraseBtnCmd();
}

NavEditState::~NavEditState() {
	std::map<short, Command*>::iterator itr = _navcmds.begin();
	for (; itr != _navcmds.end(); itr++)
		delete itr->second;
	_navcmds.clear();
}
