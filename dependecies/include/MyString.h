#ifndef _MY_STRING_H_
#define _MY_STRING_H_
#pragma warning(disable: 4100)

#include <string>
#include <iostream>
#include <fstream>
#include <iterator>
#include "ReverseIterator.h"

class MyString {
private:
	static constexpr size_t cCapacityFactor { 16 };
	char* str_;			//Storage for string symbols
	size_t size_;		//Number of non-zero symbols in string
	size_t capacity_;	//Number of non-zero symbols current allocated memory can have

	static char* zero_alloc(size_t size);					//Allocate memory and memset 0
	void check_capacity_and_realloc(size_t requiredSize);	//Realloc, if requiredSize > capacity
	bool fits_to_capacity(size_t requiredSize);				//Check if requiredSize < capacity
	void realloc_and_copy(size_t requiredSize);				//Realloc memory and copy existing string to it
public:

	class conversion_error {};					//Convertion error class

	static constexpr size_t npos = SIZE_MAX;	//Returned if find unsuccessful

	MyString();											
	MyString(const MyString& str);						//Copy constructor
	//MyString(MyString&& str);							//Move constructor
	MyString(const char* str);			
	MyString(std::initializer_list<char> init_list);
	explicit MyString(std::string str);
	MyString(const char* str, size_t count);
	MyString(size_t count, char ch);
	MyString(int number);
	MyString(float number);

	friend MyString operator+(MyString& lhs, MyString const& rhs);
	friend MyString operator+(MyString& lhs, const char* rhs);
	friend MyString operator+(MyString& lhs, std::string rhs);

	MyString& operator+=(const char* rhs);
	MyString& operator+=(std::string rhs);
	MyString& operator+= (MyString const& rhs);
	MyString& operator=(const char* rhs);
	MyString& operator=(std::string& rhs);	
	MyString& operator=(char rhs);
	MyString& operator=(MyString&& str);
	char& operator[](size_t index) const;

	friend bool operator<(MyString const& lhs, MyString const& rhs);
	friend bool operator>(MyString const& lhs, MyString const& rhs);
	friend bool operator<=(MyString const& lhs, MyString const& rhs);
	friend bool operator>=(MyString const& lhs, MyString const& rhs);
	friend bool operator==(MyString const& lhs, MyString const& rhs);
	friend bool operator!=(MyString const& lhs, MyString const& rhs);

	const char* c_str() const;
	const void* data() const;
	size_t length() const;
	size_t size() const;
	bool empty() const;
	size_t capacity() const;
	void shrink_to_fit();
	void clear();
	char& at(size_t idx);

	int to_int();
	float to_float();

	void insert(size_t index, size_t count, char ch);
	void insert(size_t index, const char* str);
	void insert(size_t index, const char* str, size_t count);
	void insert(size_t index, std::string const& str);
	void insert(size_t index, std::string const& str, size_t count);

	void erase(size_t index, size_t count);

	void append(size_t count, char ch);
	void append(const char* str);
	void append(const char* str, size_t index, size_t count);
	void append(std::string const& str);
	void append(std::string const& str, size_t index, size_t count);

	void replace(size_t index, size_t count, const char* str);
	void replace(size_t index, size_t count, std::string const& str);

	MyString substr(size_t index);
	MyString substr(size_t index, size_t count);

	size_t find(const char* str);
	size_t find(const char* str, size_t index);
	size_t find(std::string const& str);
	size_t find(std::string const& str, size_t index);

	size_t find_ac(std::string str);

	~MyString();

	friend std::ostream& operator<<(std::ostream& os, const MyString& string);
	friend std::istream& operator>>(std::istream & is, MyString & string);
	friend std::ofstream& operator<<(std::ofstream& os, const MyString& string);
	friend std::ifstream& operator>>(std::ifstream & is, MyString & string);

	class iterator : std::iterator<std::input_iterator_tag, char> 
	{
	public:
		typedef char& ref;
		typedef char* ptr;

		iterator() = default;
		iterator(char* ptr) : ptr_(ptr) {}
		~iterator() = default;

		bool operator==(const iterator& it) { return ptr_ == it.ptr_; }
		bool operator!=(const iterator& it) { return ptr_ != it.ptr_; }
		iterator operator++() { iterator it = *this; ++ptr_; return it; }
		iterator operator++(int junk) { ++ptr_; return *this; }
		iterator operator--() { iterator it = *this; --ptr_; return it; }
		iterator operator--(int junk) { --ptr_; return *this; }
		friend size_t operator-(iterator const& lhs, iterator const& rhs) { return lhs.ptr_ - rhs.ptr_; }
		friend iterator operator+(iterator const& lhs, int num) { return iterator(lhs.ptr_ + num); }
		char& operator*() { return *ptr_; }
		char* operator->() {return ptr_; }

		friend class MyString;
	private:
		char* ptr_;
	};

	class const_iterator : std::iterator<std::input_iterator_tag, const char>
	{
	public:
		typedef const char& ref;
		typedef const char* ptr;

		const_iterator() = default;
		const_iterator(MyString::iterator& it) { ptr_ = it.operator->(); }
		const_iterator(const char* ptr) : ptr_(ptr) {}
		~const_iterator() = default;

		bool operator==(const const_iterator& it) { return ptr_ == it.ptr_; }
		bool operator!=(const const_iterator& it) { return ptr_ != it.ptr_; }
		const_iterator operator++() { const_iterator it = *this; ++ptr_; return it; }
		const_iterator operator++(int junk) { ++ptr_; return *this; }
		const_iterator operator--() { const_iterator it = *this; --ptr_; return it; }
		const_iterator operator--(int junk) { --ptr_; return *this; }
		friend size_t operator-(const_iterator const& lhs, const_iterator const& rhs) { return lhs.ptr_ - rhs.ptr_; }
		friend const_iterator operator+(const_iterator const& lhs, int num) { return const_iterator(lhs.ptr_ + num); }
		const char& operator*() { return *ptr_; }
		const char* operator->() { return ptr_; }

		friend class MyString;
	private:
		const char* ptr_;
	};

	typedef ReverseIterator<iterator, char> reverse_iterator;
	typedef ReverseIterator<const_iterator, const char> const_reverse_iterator;

	MyString::iterator begin();
	MyString::iterator end();

	MyString::const_iterator cbegin();
	MyString::const_iterator cend();

	MyString::reverse_iterator rbegin();
	MyString::reverse_iterator rend();

	MyString::const_reverse_iterator rcbegin();
	MyString::const_reverse_iterator rcend();

	void insert(const_iterator index, size_t count, char ch);
	void insert(const_iterator index, const char* str);
	void insert(const_iterator index, const char* str, size_t count);
	void insert(const_iterator index, std::string const& str);
	void insert(const_iterator index, std::string const& str, size_t count);

	void erase(const_iterator index, size_t count);

	void append(const char* str, const_iterator index, size_t count);
	void append(std::string const& str, const_iterator index, size_t count);

	void replace(const_iterator index, size_t count, const char* str);
	void replace(const_iterator index, size_t count, std::string const& str);

	MyString substr(const_iterator index);
	MyString substr(const_iterator index, size_t count);

	size_t find(const char* str, const_iterator index);
	size_t find(std::string const& str, const_iterator index);
};

#endif	// _MY_STRING_H_