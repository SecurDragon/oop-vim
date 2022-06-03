#include "View.h"
#include <string>

#include "SyntaxHighlighter.h"
#include "XMLHightlighter.h"


View::View() {
	int maxY, maxX;
	pdcurses::scr::getmaxyx(maxY, maxX);
	_txtPadHeight = maxY - cStatHeight;
	_firstVisibleLineNo = 0;
	_curX = _curY = 0;
	_txtPad = new pdcurses::Pad(cMaxPadHeight, maxX);
	_statWnd = new pdcurses::Window(1, maxX, maxY - cStatHeight, 0);
	_numbersPad = new pdcurses::Pad(cMaxPadHeight, cNumberPadWidth);
}

View::~View() {
	delete _txtPad;
	delete _statWnd;
}

void View::update(BarModel* from) {
	auto cmd = from->getBtnCmd();

	
	if (from->getNum() != _showNumbers) {
		_showNumbers = !_showNumbers;
		if (_showNumbers) {
			_txtPad->resize(cMaxPadHeight, pdcurses::scr::getmaxx() - cNumberPadWidth);
		}
		else
			_txtPad->resize(cMaxPadHeight, pdcurses::scr::getmaxx());
	}
	_statWnd->mvprintw(0, 0, "%110c", ' ');
	_statWnd->mvprintw(0, pdcurses::scr::getmaxx() - 12 - 5, "%-s", cmd.c_str());
	
	auto cmd1 = from->getDotsCmd();
	_statWnd->mvprintw(0, cDotsStart, "%-30s", cmd1.c_str());

	_statWnd->mvprintw(0, cDotsStart + cDotsMax + cGapWidth, "%-40s", from->getFilename().c_str());

	_statWnd->mvprintw(0, pdcurses::scr::getmaxx() - 1, "%c", from->getState());

	_statWnd->attron(COLOR_PAIR(0x50));
	_statWnd->mvprintw(0, cDotsStart + cDotsMax + cGapWidth + cFilenameMax + cGapWidth, "%-s", from->getLastError().c_str());
	_statWnd->attroff(COLOR_PAIR(0x50));

	if (from->getSyntax())
		_highlighter = XMLHighlighter::pinstance();
	else
		_highlighter = EmptyHighlighter::pinstance();
	
	_statWnd->refresh();
}

void View::update(TextModel* from) {
	_txtPad->clear();
	_numbersPad->clear();
	auto& str = from->getText();
	auto idx = from->getIndex();
	size_t i = 0;
	size_t nl = 0;
	for(i = 0; i < str.size(); ++i) {
		if (i == idx)
			_txtPad->getyx(_curY, _curX);
		chtype color = _highlighter->getCharColor(str[i]);
		_txtPad->addch(str[i] | color);
		if(_showNumbers) {
			if (str[i] == '\n' || i == 0) {
				++nl;
				int y = _txtPad->gety();
				_numbersPad->mvprintw(y, 0, "%5u ", nl);
			}
		}
	}
	if (i == idx)
		_txtPad->getyx(_curY, _curX);
	if (_showNumbers)
		_curX += cNumberPadWidth;

	if(static_cast<unsigned>(_curY) >= _firstVisibleLineNo + _txtPadHeight) {
		_firstVisibleLineNo = _curY - _txtPadHeight + 1;
	}
	if (static_cast<unsigned>(_curY) < _firstVisibleLineNo)
		_firstVisibleLineNo = _curY;
	
	_statWnd->mvprintw(0, pdcurses::scr::getmaxx() - cLinenoWidth - cGapWidth, "%5u/%-5u", from->getCurrentLineNo(), from->getLineCount());
	this->draw();
}

void View::draw() {
	int maxX, maxY;
	pdcurses::scr::getmaxyx(maxY, maxX);
	pdcurses::scr::refresh();
	int smincol = 0;
	if (_showNumbers) {
		_numbersPad->refresh(_firstVisibleLineNo, 0, 0, 0, maxY - cStatHeight - 1, cNumberPadWidth - 1);
		smincol = cNumberPadWidth;
	}
	_txtPad->refresh(_firstVisibleLineNo, 0, 0, smincol, maxY - cStatHeight - 1, maxX - 1);
	wmove(stdscr, _curY - _firstVisibleLineNo, _curX);
	_statWnd->refresh();
}
