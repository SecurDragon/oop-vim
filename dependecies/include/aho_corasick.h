#ifndef _AHO_CORASICK_H_
#define _AHO_CORASICK_H_

#include "MyString.h"

class queue {
private:
	struct node {
		int value;
		node* next;
	};

	node* head = nullptr;
	size_t size_ = 0;
public:
	void push(int value);
	int pop();
	size_t size() { return this->size_; }
	~queue();
};

class AhoCorasickFinder {
private:
	static constexpr size_t cAlphabetSize = 255;
	int* out_;
	int* failure_;
	int** goto_;
	size_t goto_size = 0;
	size_t find_size = 0;

	int find_next_state(int curr, char next_input);
public:
	AhoCorasickFinder() = delete;
	AhoCorasickFinder(MyString& to_find);
	size_t find_in(MyString const& where);
	~AhoCorasickFinder();
};

#endif
