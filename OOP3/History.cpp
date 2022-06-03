#include "History.h"
#include <algorithm>

template<class Container, class OutIter, class UnaryPredicate>
OutIter move_and_erase_if(Container& in, OutIter out, UnaryPredicate&& predicate) {
	for (auto iter = std::find_if(in.begin(), in.end(), predicate);
		 iter != in.end();
		 iter = std::find_if(iter, in.end(), predicate))
	{
		*out = std::move(*iter);
		++out;
		iter = in.erase(iter);
	}

	return out;
}

void History::add(Memento* mem, size_t line) {
	_mementos.push_back(std::make_pair(line, mem));
}

void History::undo() {
	if (_mementos.empty())
		return;
	Memento* m = _mementos.back().second;
	_mementos.pop_back();
	m->restore();
	delete m;
}

void History::undoLine(size_t line) {
	/*if (_mementos.empty())
		return;
	std::vector<std::pair<size_t, Memento*>> line_mem;
	move_and_erase_if(_mementos, std::back_inserter(line_mem),
					  [&](const std::pair<size_t, Memento*>& element) { return element.first == line; });

	Memento* restore = line_mem.front().second;
	restore->restore();

	for (auto kv : line_mem) {
		Memento* m = kv.second;
		delete m;
	}*/
}

void History::undoAll() {
	while (!_mementos.empty()) {
		Memento* m = _mementos.back().second;
		m->restore();
		delete m;
		_mementos.pop_back();
	}
}

History::~History() {
	while (!_mementos.empty()) {
		Memento* m = _mementos.back().second;
		delete m;
		_mementos.pop_back();
	}
}
