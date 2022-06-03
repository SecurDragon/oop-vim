#include "MyStringDecorator.h"
#include <algorithm>

size_t MyStringDecorator::rfind(const char* str, size_t index) {
	if (str == nullptr) return npos;
	if (index > this->size()) return npos;
	size_t str_len = strlen(str);
	if (str_len > this->size()) return npos;

	for(auto ch = this->c_str() + std::min(index, this->size() - str_len); ; --ch) {
		if (*ch == *str &&
			strncmp(ch, str, str_len) == 0)
			return static_cast<size_t>(ch - this->c_str());
		if (ch == this->c_str())
			return npos;
	}
}

size_t MyStringDecorator::count(const char* str, size_t offset) {
	size_t count = 0;
	while(offset != npos) {
		size_t where = find(str, offset);
		if (where == npos) break;
		offset = where + 1;
		++count;
	}
	return count;
}

size_t MyStringDecorator::rcount(const char* str, size_t offset) {
	size_t count = 0;
	while (offset != npos) {
		size_t where = rfind(str, offset);
		if (where == npos) break;
		offset = where - 1;
		++count;
	}
	return count;
}
