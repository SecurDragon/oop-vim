#ifndef _REVERSE_ITERATOR_H_
#define _REVERSE_ITERATOR_H_
#include <iterator>

template<class T, typename U>
class ReverseIterator {
public:

	ReverseIterator() = default;
	ReverseIterator(T iter) : iter_(iter) {}
	~ReverseIterator() = default;

	bool operator==(const ReverseIterator& it) { return iter_ == it.iter_; }
	bool operator!=(const ReverseIterator& it) { return iter_ != it.iter_; }
	ReverseIterator operator++() { ReverseIterator it = *this; --iter_; return it; }
	ReverseIterator operator++(int junk) { --iter_; return *this; }
	ReverseIterator operator--() { ReverseIterator it = *this; --iter_; return it; }
	ReverseIterator operator--(int junk) { --iter_; return *this; }
	U& operator*() { return *iter_; }
	U* operator->() { return iter_.operator->(); }
private:
	T iter_;
};

#endif //_REVERSE_WRAPPER_H_