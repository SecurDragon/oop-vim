#ifndef _PDCURSES_WRAPPER_H_
#define _PDCURSES_WRAPPER_H_

#include <curses.h>
#include "MyStringDecorator.h"
#undef getch
#undef getmaxyx
#undef getyx
#undef ungetch

namespace pdcurses
{
	bool init();
	bool end();

	namespace scr {
		bool refresh();
		int getch();
		bool getmaxyx(int& y, int& x);
		int getmaxx();
		int getmaxy();
		bool clear();
		bool clearPixel(int y, int x);
	}
	
	namespace clipboard
	{
		MyString get();
		bool set(MyString& str);
	}
	
	class Window {
	private:
		WINDOW* _wnd;
	public:
		Window(int nlines, int ncols, int begin_y, int begin_x);
		~Window();
		bool resize(int lines, int columns);
		bool mv(int y, int x);
		bool clear();
		bool refresh();
		bool mvprintw(int y, int x, const char* fmt, ...);
		bool attron(chtype attr);
		bool attroff(chtype attr);
	};

	class Pad {
	private:
		WINDOW* _pad;
	public:
		Pad(int nlines, int ncols);
		~Pad();
		bool refresh(int pminrow, int pmincol, int sminrow, int smincol, int smaxrow, int smaxcol);
		bool mvprintw(int y, int x, const char* fmt, ...);
		bool addstr(const char* str);
		bool addch(chtype);
		bool getyx(int& y, int& x);
		int getx();
		int gety();
		bool clear();
		bool move(int y, int x);
		bool attron(chtype attr);
		bool attroff(chtype attr);
		bool resize(int newy, int newx);
	};

}

#endif //_PDCURSES_WRAPPER_H_
