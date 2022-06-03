#ifndef _MEMENTO_H_
#define _MEMENTO_H_

class Memento {
public:
	virtual void restore() = 0;
	virtual ~Memento() = default;
};

class Originator {
public:
	virtual Memento* save() = 0;
	virtual ~Originator() = default;
};

#endif //_MEMENTO_H_