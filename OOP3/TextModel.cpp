#include "TextModel.h"

TextModel::HistoryEntry::HistoryEntry(TextModel* model) 
	: _model(model){
	_line = model->getCurrentLineNo();
	size_t line_beg = model->lineBegIdx();
	size_t line_end = model->lineEndIdx();
	_text = model->_text.substr(line_beg, line_end - line_beg + 1);
}

void TextModel::HistoryEntry::restore() {
	size_t lineIdx = _model->getLineIdx(_line) + 1;
	if (lineIdx != MyString::npos) {
		_model->_currentIndex = lineIdx;
		size_t line_end = _model->lineEndIdx();
		_model->_text.erase(lineIdx, line_end - lineIdx + 1);
		_model->_text.insert(lineIdx, _text.c_str());
	}
	_model->notify();
}

Memento* TextModel::save() {
	return new HistoryEntry(this);
}
TextModel::TextModel(MyString const& str) : _text(str.c_str()), _currentIndex(0) {
	notify();
}

TextModel::TextModel(MyString const str) : _text(str.c_str()), _currentIndex(0)
{
	notify();
}

size_t TextModel::getIndex() const {
	return this->_currentIndex;
}

void TextModel::setIndex(size_t idx) {
	if (idx >= _text.size())
		idx = _text.size() - 1;
	if (_text.size() == 0)
		idx = 0;
	this->_currentIndex = idx;
	notify();
}

size_t TextModel::getLineCount(){
	try {
		return this->_text.count("\n", 0) + 1;
	}
	catch(...) {
		return 0;
	}
}

size_t TextModel::getCurrentLineNo() {
	return this->_text.rcount("\n", _currentIndex - 1) + 1;
}

void TextModel::append(char ch) {
	this->_text.insert(_currentIndex++, 1, ch);
	notify();
}

void TextModel::erase(size_t count) {
	if (this->_text.size() == 0) return;
	if (_currentIndex >= this->_text.size()) return;
	
	this->_text.erase(_currentIndex, count);
	notify();
}

MyStringDecorator& TextModel::getText(){
	return this->_text;
}

void TextModel::setText(MyStringDecorator const& str) {
	_text = str.c_str();
	notify();
}

size_t TextModel::lineBegIdx() {
	size_t beg = MyString::npos;
	try {
		beg = _text.rfind("\n", _currentIndex);
	}
	catch (...) {}
	if (beg == MyStringDecorator::npos)
		beg = static_cast<size_t>(-1);
	return beg + 1;
}

size_t TextModel::lineEndIdx() {
	size_t beg = MyString::npos;
	try {
		beg = _text.find("\n", _currentIndex);
	}
	catch (...) {}
	if (beg == MyStringDecorator::npos)
		beg = _text.size();
	return beg - 1;
}

size_t TextModel::getLineIdx(size_t line) {
	size_t idx = 0;
	size_t currline = 0;
	while (currline != line - 1) {
		idx = _text.find("\n", idx + 1);
		if (idx >= _text.size()) {
			idx = MyString::npos;
			break;
		}
		++currline;
	}
	return idx;
}


