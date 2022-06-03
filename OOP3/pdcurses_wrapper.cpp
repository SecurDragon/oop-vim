#include "pdcurses_wrapper.h"
#include <curspriv.h>

bool pdcurses::init() {
	initscr();
	int res = 0;
	res |= noecho();
	//res |= notimeout(stdscr, TRUE);
	res |= curs_set(1);
	res |= keypad(stdscr, TRUE);
	res |= start_color();
	attron(A_ALTCHARSET);
	init_pair(0x50, COLOR_WHITE, COLOR_RED);
	init_pair(0x100, COLOR_YELLOW, COLOR_RED);
	return res == OK;
}

bool pdcurses::end() {
	endwin();
	return true;
}

MyString pdcurses::clipboard::get() {
	char* content = NULL;
	long length;
	PDC_getclipboard(&content, &length);
	MyString str(content);
	PDC_freeclipboard(content);
	return str;
}

bool pdcurses::clipboard::set(MyString& str) {
	return OK == PDC_setclipboard(str.c_str(), str.size());
}

bool pdcurses::scr::refresh() {
	return OK == wrefresh(stdscr);
}

int pdcurses::scr::getch() {
	return wgetch(stdscr);
}

bool pdcurses::scr::getmaxyx(int& y, int& x) {
	y = pdcurses::scr::getmaxy();
	x = pdcurses::scr::getmaxx();
	return true;
}

int pdcurses::scr::getmaxx() {
	return getmaxx(stdscr);
}

int pdcurses::scr::getmaxy() {
	return getmaxy(stdscr);
}

bool pdcurses::scr::clear() {
	return wclear(stdscr);
}

bool pdcurses::scr::clearPixel(int y, int x) {
	return OK == mvwprintw(stdscr, y, x, " ");
}

pdcurses::Window::Window(int nlines, int ncols, int begin_y, int begin_x) {
	_wnd = newwin(nlines, ncols, begin_y, begin_x);
}

pdcurses::Window::~Window() {
	delwin(this->_wnd);
}

bool pdcurses::Window::resize(int lines, int columns) {
	return OK == wresize(this->_wnd, lines, columns);
}

bool pdcurses::Window::mv(int y, int x) {
	return OK == mvwin(this->_wnd, y, x);
}

bool pdcurses::Window::clear() {
	return OK == wclear(this->_wnd);
}

bool pdcurses::Window::refresh() {
	return OK == wrefresh(this->_wnd);
}

bool pdcurses::Window::mvprintw(int y, int x, const char* fmt, ...) {	
	va_list args;
	va_start(args, fmt);
	wmove(this->_wnd, y, x);
	int res = vwprintw(this->_wnd, fmt, args);
	va_end(args);
	
	return OK == res;
}

bool pdcurses::Window::attron(chtype attr) {
	return OK == wattron(this->_wnd, attr);
}

bool pdcurses::Window::attroff(chtype attr) {
	return OK == wattroff(this->_wnd, attr);
}

pdcurses::Pad::Pad(int nlines, int ncols) {
	this->_pad = newpad(nlines, ncols);
}

pdcurses::Pad::~Pad() {
	delwin(this->_pad);
}

bool pdcurses::Pad::refresh(int pminrow, int pmincol, int sminrow, int smincol, int smaxrow, int smaxcol) {
	return OK == prefresh(this->_pad, pminrow, pmincol, sminrow, smincol, smaxrow, smaxcol);
}

bool pdcurses::Pad::mvprintw(int y, int x, const char* fmt, ...) {
	va_list args;
	va_start(args, fmt);
	wmove(this->_pad, y, x);
	int res = vwprintw(this->_pad, fmt, args);
	va_end(args);

	return OK == res;
}

bool pdcurses::Pad::addstr(const char* str) {
	return OK == waddstr(this->_pad, str);
}

bool pdcurses::Pad::addch(chtype ch) {
	return OK == waddch(this->_pad, ch);
}

bool pdcurses::Pad::getyx(int& y, int& x) {
	y = getcury(this->_pad);
	x = getcurx(this->_pad);
	return true;
}

int pdcurses::Pad::getx() {
	return getcurx(this->_pad);
}

int pdcurses::Pad::gety() {
	return getcury(this->_pad);
}

bool pdcurses::Pad::clear() {
	return OK == wclear(_pad);
}

bool pdcurses::Pad::move(int y, int x) {
	return OK == wmove(_pad, y, x);
}

bool pdcurses::Pad::attron(chtype attr) {
	return OK == wattroff(this->_pad, attr);
}

bool pdcurses::Pad::attroff(chtype attr) {
	return OK == wattroff(this->_pad, attr);
}

bool pdcurses::Pad::resize(int newy, int newx) {
	return OK == wresize(this->_pad, newy, newx);
}
