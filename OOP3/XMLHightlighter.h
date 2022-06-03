#ifndef _XML_HIGHLIGHTER_H_
#define _XML_HIGHLIGHTER_H_
#include "SyntaxHighlighter.h"

class XMLHighlighter :
	public SyntaxHighlighter,
	public Singleton<XMLHighlighter>
{
private:
	bool _in_tag = false;
	bool _in_attrname = false;
	bool _in_attrval = false;
	bool _was_in_attrval = false;
	enum class Colors {
		BLANK = 0,
		TAG = 0x100,
		ATTR_KEY,
		ATTR_VAL
	};
public:
	XMLHighlighter(guard);
	chtype getCharColor(char ch) override;
};

#endif //_XML_HIGHLIGHTER_H_