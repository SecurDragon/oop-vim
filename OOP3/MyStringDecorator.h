#ifndef _MYSTRING_DECORATOR_H_
#define _MYSTRING_DECORATOR_H_

#pragma warning(disable:4100)

#include <MyString.h>
class MyStringDecorator :
	public MyString
{
public:
	using MyString::MyString;

	size_t rfind(const char* str, size_t index);
	size_t count(const char* str, size_t offset);
	size_t rcount(const char* str, size_t offset);

};

#endif //_MYSTRING_DECORATOR_H_