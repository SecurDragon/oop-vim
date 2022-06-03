#include "DotCmdState.h"

Command* DotCmdState::getCommand(MyString& str) {
	auto fnd = _cmds.find(str.c_str());
	if (fnd != _cmds.end())
		return fnd->second;
	fnd = _cmds.find(str.substr(0, 1).c_str());
	if (fnd != _cmds.end())
		return fnd->second;
	try {
		int num = str.to_int();
		reinterpret_cast<GotoLineCommand*>(_cmds["\bgoto"])->setNum(num);
		return _cmds["\bgoto"];
	}
	catch (...) {}
	return new EmptyCommand();
}

DotCmdState::DotCmdState(guard, TextModel* text, BarModel* bar): _txtMdl(text), _barMdl(bar) {
	bar->setState('C'); // hello
	_cmds.insert(std::make_pair("q!", new LambdaCommand([&]() {_ctrl->setStop(); })));
	_cmds.insert(std::make_pair("\bgoto", new GotoLineCommand(*_txtMdl)));
	_cmds.insert(std::make_pair("o", new LambdaCommand([&]() {
		MyString arg;
		try {
			arg = _barMdl->getDotsCmd().substr(3);
		}
		catch(...) {
			arg = "<<<";
		}
		OpenFileCommand(*_txtMdl, *_barMdl, arg).execute();
	})));
	_cmds.insert(std::make_pair("q", new LambdaCommand([&]() {
		if (!_barMdl->modified())
			_ctrl->setStop();
		else
			_barMdl->setLastError("ERR: Unsaved changes");
	})));
	_cmds.insert(std::make_pair("x", new LambdaCommand([&]() {
		WriteToFileCommand(*_txtMdl, *_barMdl, _barMdl->getFilename().c_str()).execute();
		if (_barMdl->getLastError().empty())
			_ctrl->setStop();
	})));
	_cmds.insert(std::make_pair("wq!", new LambdaCommand([&]() {
		WriteToFileCommand(*_txtMdl, *_barMdl, _barMdl->getFilename().c_str()).execute();
		if (_barMdl->getLastError().empty())
			_ctrl->setStop();
	})));
	_cmds.insert(std::make_pair("set num", new LambdaCommand([&]() {
		_barMdl->toggleNumLines();
		_txtMdl->notify();
	})));
	_cmds.insert(std::make_pair("sy", new LambdaCommand([&]() {
		_barMdl->toggleSyntax();
		_txtMdl->notify();
	})));
	_cmds.insert(std::make_pair("w", new LambdaCommand([&]() {
		MyString arg;
		try {
			arg = _barMdl->getDotsCmd().substr(3);
		}
		catch (...) {
			arg = "<<<";
		}
		WriteToFileCommand(*_txtMdl, *_barMdl, _barMdl->getDotsCmd().size() == 2 ? _barMdl->getFilename().c_str() : arg).execute();
	})));
}

void DotCmdState::update(short key) {
	if (isprint(static_cast<char>(key)))
		_barMdl->appendDotsCmd(static_cast<char>(key));

	if (key == 10) {
		auto cmd_s = _barMdl->getDotsCmd().substr(1);
		Command* cmd = getCommand(cmd_s);
		if (cmd != nullptr) {
			_barMdl->setLastError("");
			cmd->execute();
		}
	}

	if (key == 27 || key == 10) {
		_barMdl->setDotsCmd("");
		_barMdl->setState('N');
		_ctrl->setState(NavEditState::pinstance(_txtMdl, _barMdl));
	}
}
