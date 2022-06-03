#include "BarModel.h"

void BarModel::setLastError(MyStringDecorator err) {
	_lastError = err.c_str();
	notify();
}

MyStringDecorator BarModel::getLastError() {
	return _lastError;
}

void BarModel::setState(char ch) {
	_currState = ch;
	notify();
}

char BarModel::getState() {
	return _currState;
}

void BarModel::setFilename(MyStringDecorator err) {
	_currFile = err.c_str();
	notify();
}

MyStringDecorator BarModel::getFilename() {
	return _currFile;
}

bool BarModel::modified() {
	return _wasModified;
}

void BarModel::setModified() {
	_wasModified = true;
}

MyStringDecorator BarModel::getBtnCmd() {
	return _btnCmd;
}

void BarModel::eraseBtnCmd() {
	_btnCmd.clear();
	notify();
}

void BarModel::appendBtnCmd(char ch) {
	_btnCmd.append(1, ch);
	notify();
}

MyStringDecorator BarModel::getDotsCmd() {
	return _dotsCmd;
}

void BarModel::setDotsCmd(const char* str) {
	_dotsCmd = str;
	notify();
}

void BarModel::appendDotsCmd(char ch) {
	_dotsCmd.append(1, ch);
	notify();
}

void BarModel::setFind(MyString tofind) {
	_lastSearch = tofind.c_str();
	notify();
}

MyStringDecorator BarModel::getLastSearch() {
	return _lastSearch;
}

void BarModel::toggleNumLines() {
	_showLines = !_showLines;
	notify();
}

bool BarModel::getNum() {
	return _showLines;
}

void BarModel::toggleSyntax() {
	_showSyntax = !_showSyntax;
	notify();
}

bool BarModel::getSyntax() {
	return _showSyntax;
}
