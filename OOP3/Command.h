#ifndef _COMMAND_H_
#define _COMMAND_H_

#include <functional>

#include "TextModel.h"
#include "BarModel.h"
#include "History.h"

class Command {
public:
	Command() = default;
	virtual void execute() = 0;
	virtual ~Command() = default;
};

class ModifyCommand :
	public Command
{
protected:
	TextModel& _mdl;
public:
	ModifyCommand(TextModel& mdl) : _mdl(mdl){}
};

class EmptyCommand:
	public Command
{
public:
	void execute() override {
		
	}
};

class PrintCommand:
	public ModifyCommand
{
private:
	char _ch;
public:
	PrintCommand(TextModel& mdl, char ch):
		ModifyCommand(mdl), _ch(ch){}
	void execute() override {
		_mdl.append(_ch);
	}
};

class BackspaceEraseCommand:
	public ModifyCommand
{
public:
	using ModifyCommand::ModifyCommand;
	void execute() override{
		size_t idx = _mdl.getIndex();
		if (idx != 0) {
			_mdl.setIndex(idx - 1);
			_mdl.erase(1);
		}
	}
};

class DeleteEraseCommand:
	public ModifyCommand
{
public:
	using ModifyCommand::ModifyCommand;
	void execute() override {
		_mdl.erase(1);
	}
};

class MoveCursorCommand:
	public ModifyCommand {
public:
	enum class CursorDirection {
		UP, DOWN, LEFT, RIGHT, SCR_UP, SCR_DOWN
	};
	
	MoveCursorCommand(TextModel& mdl, CursorDirection dir, int num = 1):
		ModifyCommand(mdl), _dir(dir), _num(num){}	
	void execute() override {
		auto& text = _mdl.getText();
		size_t curr_idx = _mdl.getIndex();

		try {
			switch (_dir) {
			case CursorDirection::RIGHT:
			{
				size_t end = text.find("\n", curr_idx);

				size_t new_idx = curr_idx + 1;
				if (new_idx <= end)
					_mdl.setIndex(new_idx);
				break;
			}
			case CursorDirection::DOWN:
			{
				size_t start = text.rfind("\n", curr_idx) + 1;
				size_t next_start = text.find("\n", curr_idx) + 1;
				if (next_start == 0) return;
				size_t next_end = text.find("\n", next_start);

				size_t offset = curr_idx - start;
				size_t new_idx = next_start + offset;

				if (new_idx >= next_end)
					new_idx = next_end - 1;
				if (new_idx < next_start)
					new_idx = next_start;

				_mdl.setIndex(new_idx);
				break;
			}
			case CursorDirection::LEFT: {
				size_t start = text.rfind("\n", curr_idx - 1) + 1;

				size_t new_idx = curr_idx - 1;
				if (start == MyString::npos)
					new_idx = 0;
				if (new_idx >= start)
					_mdl.setIndex(new_idx);
				break;
			}
			case CursorDirection::UP:
			{
				size_t prev_end = text.rfind("\n", curr_idx - 1);
				if (prev_end == MyString::npos || curr_idx == 0) return;
				size_t start = prev_end + 1;
				size_t prev_start = text.rfind("\n", prev_end - 1) + 1;

				size_t offset = curr_idx - start;
				size_t new_idx = prev_start + offset;

				if (new_idx >= prev_end)
					new_idx = prev_end - 1;
				if (new_idx < prev_start)
					new_idx = prev_start;

				_mdl.setIndex(new_idx);
				break;
			}
			case CursorDirection::SCR_UP:
			{
				Command* dummy = new MoveCursorCommand(_mdl, CursorDirection::UP);
				for (int i = 0; i < _num * 2; ++i)
					dummy->execute();
				delete dummy;
				break;
			}
			case CursorDirection::SCR_DOWN:
			{
				Command* dummy = new MoveCursorCommand(_mdl, CursorDirection::DOWN);
				for (int i = 0; i < _num * 2; ++i)
					dummy->execute();
				delete dummy;
				break;
			}
			}
		}
		catch (...) { return; }
	}
private:
	CursorDirection _dir;
	int _num;
};

class LambdaCommand:
	public Command
{
	std::function<void(void)> _lambda;
public:
	LambdaCommand(std::function<void(void)> lambda) : _lambda(lambda){}
	void execute() override {
		return _lambda();
	}
};

class GotoLineBegCommand:
	public ModifyCommand
{
public:
	using ModifyCommand::ModifyCommand;
	void execute() override {
		auto& text = _mdl.getText();
		size_t line = text.rfind("\n", _mdl.getIndex());
		if (line != MyString::npos || _mdl.getCurrentLineNo() == 1)
			_mdl.setIndex(line + 1);
	}
};

class GotoLineCommand:
	public ModifyCommand {
private:
	size_t linenum = MyString::npos;
public:
	using ModifyCommand::ModifyCommand;
	void setNum(size_t num) {
		linenum = num;
	}
	void execute() override {
		if(linenum != MyString::npos || linenum <= _mdl.getLineCount()) {
			auto& text = _mdl.getText();
			size_t idx = 0;
			size_t currline = 0;
			while(currline != linenum - 1) {
				idx = text.find("\n", idx + 1);
				if (idx >= text.size())
					return;
				++currline;
			}
			_mdl.setIndex(idx + 1);
		}
	}
};

class GotoLineEndCommand:
	public ModifyCommand
{
public:
	using ModifyCommand::ModifyCommand;
	void execute() override {
		auto& text = _mdl.getText();
		size_t line = text.find("\n", _mdl.getIndex());
		if (_mdl.getCurrentLineNo() == _mdl.getLineCount())
			line = text.size();
		if (line != MyString::npos)
			_mdl.setIndex(line - 1);
	}
};

class GotoTextBegCommand:
	public ModifyCommand
{
public:
	using ModifyCommand::ModifyCommand;
	void execute() override {
		_mdl.setIndex(0);
	}
};

class GotoTextEndCommand :
	public ModifyCommand
{
public:
	using ModifyCommand::ModifyCommand;
	void execute() override {
		_mdl.setIndex(_mdl.getText().size() - 1);
	}
};

class GotoNextWordCommand:
	public ModifyCommand {
private:
	bool _tospace = false;
public:
	GotoNextWordCommand(TextModel& mdl, bool gotospace = false):
		ModifyCommand(mdl), _tospace(gotospace) {}
	void execute() override {
		auto& text = _mdl.getText();
		size_t idx = _mdl.getIndex(), next_space = idx;

		do {
			idx = next_space;
			next_space = text.find(" ", idx + 1);
			if (next_space == MyString::npos) break;
		} while (next_space == idx + 1);

		if (next_space == MyString::npos)
			next_space = text.size() + (_tospace ? -1 : 0);
		
		if (!_tospace)
			--next_space;

		_mdl.setIndex(next_space);
	}
};

class GotoPrevWordCommand:
	public ModifyCommand
{
private:
	bool _tospace = false;
public:
	GotoPrevWordCommand(TextModel& mdl, bool gotospace = false):
		ModifyCommand(mdl), _tospace(gotospace) {}
	void execute() override {
		auto& text = _mdl.getText();
		size_t idx = _mdl.getIndex(), next_space = idx;

		do {
			idx = next_space;
			next_space = text.rfind(" ", idx - 1);
			if (next_space == MyString::npos) break;
		} while (next_space == idx - 1);

		if (next_space == MyString::npos)
			next_space = 0 + (_tospace ? 0 : -1);

		if (!_tospace)
			++next_space;

		_mdl.setIndex(next_space);
	}
};

class DeleteToWEndCommand:
	public ModifyCommand {
private:
	bool _withspace = false;
public:
	DeleteToWEndCommand(TextModel& mdl, bool withspace = false) : ModifyCommand(mdl), _withspace(withspace) {}
	void execute() override {
		History::instance().add(_mdl.save(), _mdl.getCurrentLineNo());
		auto& text = _mdl.getText();
		size_t idx = _mdl.getIndex();
		size_t space = text.find(" ", idx);
		if (space == MyString::npos)
			space = text.size() - 1 + (_withspace ? -1 : 0);
		if (_withspace) ++space;

		_mdl.erase(space - idx);
	}
};

class DeleteWordCommand :
	public ModifyCommand
{
public:
	using ModifyCommand::ModifyCommand;
	void execute() override {
		History::instance().add(_mdl.save(), _mdl.getCurrentLineNo());
		auto& text = _mdl.getText();
		size_t idx = _mdl.getIndex();

		size_t prev_line = text.rfind(" ", idx);
		size_t next_line = text.find(" ", idx);
		if (prev_line == MyString::npos)
			prev_line = 0;
		if (next_line == MyString::npos)
			next_line = text.size() - 1;
		text.erase(prev_line, next_line - prev_line + 1);
		_mdl.setIndex(idx);
	}
};

class JunctionCommand :
	public ModifyCommand
{
public:
	using ModifyCommand::ModifyCommand;
	void execute() override {
		History::instance().add(_mdl.save(), _mdl.getCurrentLineNo());
		auto& text = _mdl.getText();
		size_t idx = _mdl.getIndex();

		try {
			size_t nl = text.find("\n", idx);
			text.erase(nl, 1);
			_mdl.setIndex(idx);
		}
		catch (...){}
	}
};

class DelToLineEndCommand :
	public ModifyCommand
{
public:
	using ModifyCommand::ModifyCommand;
	void execute() override {
		History::instance().add(_mdl.save(), _mdl.getCurrentLineNo());
		auto& text = _mdl.getText();
		size_t idx = _mdl.getIndex();
		try {
			size_t nl = text.find("\n", idx);
			if (nl == MyString::npos)
				nl = text.size() - 1;
			text.erase(idx, nl - idx + 1);
			_mdl.setIndex(idx - 1);
		}catch(...){}
	}
};

class DelToLineBegCommand :
	public ModifyCommand
{
public:
	using ModifyCommand::ModifyCommand;
	void execute() override {
		History::instance().add(_mdl.save(), _mdl.getCurrentLineNo());
		auto& text = _mdl.getText();
		size_t idx = _mdl.getIndex();
		try {
			size_t nl = text.rfind("\n", idx);
			if (nl == MyString::npos)
				nl = 0;
			text.erase(nl, idx - nl);
			_mdl.setIndex(nl);
		}
		catch (...) {}
	}
};

class EraseCurrLineCommand :
	public ModifyCommand {
public:
	using ModifyCommand::ModifyCommand;
	void execute() override {
		History::instance().add(_mdl.save(), _mdl.getCurrentLineNo());
		auto& text = _mdl.getText();
		size_t idx = _mdl.getIndex();

		size_t prev_line = text.rfind("\n", idx);
		size_t next_line = text.find("\n", idx);
		if (prev_line == MyString::npos)
			prev_line = static_cast<size_t>(-1);
		if (next_line == MyString::npos)
			next_line = text.size();
		text.erase(prev_line + 1, next_line - prev_line - 1);
		_mdl.setIndex(prev_line + 1);
	}
};

class WriteToFileCommand :
	public ModifyCommand
{
private:
	MyStringDecorator _filename;
	BarModel& _bar;
public:
	WriteToFileCommand(TextModel& mdl, BarModel& bar, MyString fname) :
		ModifyCommand(mdl), _filename(fname.c_str()), _bar(bar) {}
	void execute() override {
		std::ofstream f;
		f.open(_filename.c_str());
		if (!f.is_open())
			return _bar.setLastError(MyStringDecorator("ERR: No such file"));
		f.write(_mdl.getText().c_str(), _mdl.getText().size());
		f.close();
		_bar.setFilename(_filename);
	}
};

class OpenFileCommand :
	public ModifyCommand {
private:
	MyStringDecorator _filename;
	BarModel& _bar;
public:
	OpenFileCommand(TextModel& mdl, BarModel& bar, MyString fname) :
		ModifyCommand(mdl), _filename(fname.c_str()), _bar(bar) {}
	void execute() override {
		std::ifstream f;
		f.open(_filename.c_str());
		if (!f.is_open())
			return _bar.setLastError("ERR: No such file");
		std::string str((std::istreambuf_iterator<char>(f)),
						std::istreambuf_iterator<char>());
		_mdl.setText(MyStringDecorator(str));
		f.close();
		_bar.setFilename(_filename);
	}
};

class FindToEndCommand:
	public ModifyCommand {
private:
	MyStringDecorator _to_find;
public:
	using ModifyCommand::ModifyCommand;
	void setText(MyStringDecorator to_find) {
		_to_find = to_find.c_str();
	}
	void execute() override {
		auto& text = _mdl.getText();
		size_t idx = _mdl.getIndex();
		try {
			size_t newIdx = text.find(_to_find.c_str(), idx + 1);
			if (newIdx != MyString::npos)
				idx = newIdx;
		}catch(...){}
		_mdl.setIndex(idx);
	}
};

class FindToBegCommand :
	public ModifyCommand {
private:
	MyStringDecorator _to_find;
public:
	using ModifyCommand::ModifyCommand;
	void setText(MyStringDecorator to_find) {
		_to_find = to_find.c_str();
	}
	void execute() override {
		auto& text = _mdl.getText();
		size_t idx = _mdl.getIndex();
		try {
			size_t newIdx = text.rfind(_to_find.c_str(), idx - 1);
			if (newIdx != MyString::npos)
				idx = newIdx;
		}
		catch (...) {}
		_mdl.setIndex(idx);
	}
};

#include "pdcurses_wrapper.h"

class PutCBBeforeCommand :
	public ModifyCommand
{
public:
	using ModifyCommand::ModifyCommand;
	void execute() override {
		auto& text = _mdl.getText();
		size_t idx = _mdl.getIndex();

		MyString str = pdcurses::clipboard::get();
		text.insert(idx, str.c_str());
		_mdl.setIndex(idx + str.size() - 1);
	}
};

class PutCBAfterCommand :
	public ModifyCommand
{
public:
	using ModifyCommand::ModifyCommand;
	void execute() override {
		auto& text = _mdl.getText();
		size_t idx = _mdl.getIndex();

		MyString str = pdcurses::clipboard::get();
		text.insert(idx + 1, str.c_str());
		_mdl.setIndex(idx + str.size());
	}
};

class CutCurrLineCommand:
	public ModifyCommand {
public:
	using ModifyCommand::ModifyCommand;
	void execute() override {
		auto& text = _mdl.getText();
		size_t idx = _mdl.getIndex();

		size_t prev_line = text.rfind("\n", idx);
		size_t next_line = text.find("\n", idx);
		if (prev_line == MyString::npos)
			prev_line = 0;
		if (next_line == MyString::npos)
			next_line = text.size() - 1;
		MyString sub = text.substr(prev_line, next_line - prev_line + 1);
		text.erase(prev_line, next_line - prev_line + 1);
		pdcurses::clipboard::set(sub);
		_mdl.setIndex(idx);
	}
};

class CopyCurrLineCommand :
	public ModifyCommand {
public:
	using ModifyCommand::ModifyCommand;
	void execute() override {
		auto& text = _mdl.getText();
		size_t idx = _mdl.getIndex();

		size_t prev_line = text.rfind("\n", idx);
		size_t next_line = text.find("\n", idx);
		if (prev_line == MyString::npos)
			prev_line = 0;
		if (next_line == MyString::npos)
			next_line = text.size() - 1;
		MyString sub = text.substr(prev_line, next_line - prev_line + 1);
		pdcurses::clipboard::set(sub);
		_mdl.setIndex(idx);
	}
};

class CopyWordCommand :
	public ModifyCommand
{
public:
	using ModifyCommand::ModifyCommand;
	void execute() override {
		auto& text = _mdl.getText();
		size_t idx = _mdl.getIndex();

		size_t prev_line = text.rfind(" ", idx);
		size_t next_line = text.find(" ", idx);
		if (prev_line == MyString::npos)
			prev_line = 0;
		if (next_line == MyString::npos)
			next_line = text.size() - 1;
		MyString sub = text.substr(prev_line, next_line - prev_line + 1);
		pdcurses::clipboard::set(sub);
	}
};

class CopyToLineBegCommand :
	public ModifyCommand
{
public:
	using ModifyCommand::ModifyCommand;
	void execute() override {
		auto& text = _mdl.getText();
		size_t idx = _mdl.getIndex();

		size_t prev_line = text.rfind("\n", idx);
		if (prev_line == MyString::npos)
			prev_line = 0;
		MyString sub = text.substr(prev_line, idx - prev_line + 1);
		pdcurses::clipboard::set(sub);
	}
};

class CopyToLineEndCommand :
	public ModifyCommand
{
public:
	using ModifyCommand::ModifyCommand;
	void execute() override {
		auto& text = _mdl.getText();
		size_t idx = _mdl.getIndex();

		size_t next_line = text.find("\n", idx);
		if (next_line == MyString::npos)
			next_line = text.size() - 1;
		MyString sub = text.substr(idx, next_line - idx + 1);
		pdcurses::clipboard::set(sub);
	}
};

#endif //_COMMAND_H_