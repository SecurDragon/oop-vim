#ifndef _SYNTAX_HIGHLIGHTER_H_
#define _SYNTAX_HIGHLIGHTER_H_

#pragma warning(disable:4100)

#include "pdcurses_wrapper.h"
#include "Singleton.h"

class SyntaxHighlighter {
public:
	virtual chtype getCharColor(char ch) = 0;
	virtual ~SyntaxHighlighter() = default;
};

class EmptyHighlighter:
	public SyntaxHighlighter,
	public Singleton<EmptyHighlighter>
{
public:
	EmptyHighlighter(guard){}
	chtype getCharColor(char ch) override {
		return 0;
	}
	
};
#endif //_SYNTAX_HIGHLIGHTER_H_