#ifndef _BARMODEL_H_
#define _BARMODEL_H_

#include "MyStringDecorator.h"
#include "Observable.h"

class BarModel:
	public Observable<BarModel>
{
private:
	MyStringDecorator _btnCmd;
	MyStringDecorator _dotsCmd;
	MyStringDecorator _currFile;
	bool _wasModified = false;
	bool _showLines = false;
	bool _showSyntax = false;
	MyStringDecorator _lastError;
	MyStringDecorator _lastSearch;
	char _currState = '0';
public:
	BarModel() = default;
	~BarModel() = default;

	void setLastError(MyStringDecorator err);

	MyStringDecorator getLastError();

	void setState(char ch);

	char getState();

	void setFilename(MyStringDecorator err);

	MyStringDecorator getFilename();

	bool modified();

	void setModified();

	MyStringDecorator getBtnCmd();

	void eraseBtnCmd();

	void appendBtnCmd(char ch);

	MyStringDecorator getDotsCmd();
	void setDotsCmd(const char* str);

	void appendDotsCmd(char ch);

	void setFind(MyString tofind);
	MyStringDecorator getLastSearch();

	void toggleNumLines();

	bool getNum();

	void toggleSyntax();

	bool getSyntax();
};

#endif //_BARMODEL_H_