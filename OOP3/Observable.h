#ifndef _OBSERVABLE_H_
#define _OBSERVABLE_H_

#include "Observer.h"
#include <vector>

template <class T>
class Observable
{
private:
	std::vector<Observer<T> *> _observers;
public:
	Observable() = default;
	virtual ~Observable() = default;
	void attach(Observer<T>* observer) {
		_observers.push_back(observer);
	}
	void notify() {
		for (auto obs : _observers)
			obs->update(static_cast<T *>(this));
	}
};

#endif //_OBSERVABLE_H_