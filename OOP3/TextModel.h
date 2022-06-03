#ifndef _TEXTMODEL_H_
#define _TEXTMODEL_H_

#include "Observable.h"
#include "Memento.h"
#include "MyString.h"
#include "MyStringDecorator.h"

class TextModel:
	public Observable<TextModel>,
	public Originator
{
private:
	MyStringDecorator _text;
	size_t _currentIndex;
public:
	
	class HistoryEntry: public Memento {
	private:
		TextModel* _model;
		MyString _text;
		size_t _line;
	public:
		HistoryEntry(TextModel* model);
		void restore() override;
		~HistoryEntry() override = default;
	};

	Memento* save() override;

	TextModel(MyString const& str);
	TextModel(MyString const str = " ");
	~TextModel() override = default;
	
	size_t getIndex() const;
	void setIndex(size_t idx);
	size_t getLineCount();
	size_t getCurrentLineNo();

	void append(char ch);
	void erase(size_t count);

	MyStringDecorator& getText();
	void setText(MyStringDecorator const& str);

	size_t lineBegIdx();
	size_t lineEndIdx();

	size_t getLineIdx(size_t line);
};

#endif //_TEXTMODEL_H_