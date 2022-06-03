#pragma once
#include "BarModel.h"
#include "Observer.h"
#include "TextModel.h"

#include "pdcurses_wrapper.h"
#include "SyntaxHighlighter.h"

class View:
	public Observer<TextModel>,
	public Observer<BarModel>
{
private:
	pdcurses::Pad* _txtPad;
	pdcurses::Window* _statWnd;
	pdcurses::Pad* _numbersPad;
	unsigned _txtPadHeight;
	unsigned _firstVisibleLineNo;
	void draw();
	int _curY, _curX;
	bool _showNumbers = false;

	SyntaxHighlighter* _highlighter = EmptyHighlighter::pinstance();
	
	constexpr static unsigned cStatHeight = 1;
	constexpr static unsigned cNumberPadWidth = 6;
	constexpr static unsigned cMaxPadHeight = 10000;
	constexpr static unsigned cDotsMax = 32;
	constexpr static unsigned cDotsStart = 0;
	constexpr static unsigned cGapWidth = 2;
	constexpr static unsigned cFilenameMax = 40;
	constexpr static unsigned cLinenoWidth = 11;
public:
	View();
	~View();
	void update(BarModel* from) override;
	void update(TextModel* from) override;
};

