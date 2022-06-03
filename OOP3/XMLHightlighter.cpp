#include "XMLHightlighter.h"
XMLHighlighter::XMLHighlighter(guard) {
	init_pair(static_cast<int>(Colors::BLANK), COLOR_WHITE, COLOR_BLACK);
	init_pair(static_cast<int>(Colors::TAG), COLOR_GREEN, COLOR_BLACK);
	init_pair(static_cast<int>(Colors::ATTR_KEY), COLOR_MAGENTA, COLOR_BLACK);
	init_pair(static_cast<int>(Colors::ATTR_VAL), COLOR_YELLOW, COLOR_BLACK);
}

chtype XMLHighlighter::getCharColor(char ch) {

	if (ch == '<')
		_in_tag = true;

	if (_in_tag && ch == ' ') {
		_in_attrname = true;
	}

	if (_in_attrval && ch == '"') {
		_in_attrval = false;
		_in_tag = true;
		_was_in_attrval = true;
	}

	if (_in_attrname && ch == '"') {
		_in_attrname = false;
		_in_attrval = true;
	}

	if (_was_in_attrval) {
		_was_in_attrval = false;
		return COLOR_PAIR(static_cast<int>(Colors::ATTR_VAL));
	}
	chtype ret = 0;
	if (_in_attrval)
		ret = COLOR_PAIR(static_cast<int>(Colors::ATTR_VAL));
	else if (_in_attrname)
		ret = COLOR_PAIR(static_cast<int>(Colors::ATTR_KEY));
	else if (_in_tag)
		ret = COLOR_PAIR(static_cast<int>(Colors::TAG));
	else
		ret = COLOR_PAIR(static_cast<int>(Colors::BLANK));

	if (ch == '>')
		_in_tag = false;


	return ret;
}
