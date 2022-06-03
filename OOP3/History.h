#ifndef _HISTORY_H_
#define _HISTORY_H_

#include <vector>

#include "Memento.h"
#include "Singleton.h"

class History:
	public Singleton<History>
{
protected:
	std::vector<std::pair<size_t, Memento*>> _mementos;
	
public:
	History(guard) {}
	void add(Memento* mem, size_t line);
	void undo();
	void undoLine(size_t line);

	void undoAll();
	
	~History();
};

#endif //_HISTORY_H_