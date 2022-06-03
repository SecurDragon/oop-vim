#ifndef _SINGLETON_H_
#define _SINGLETON_H_
#include <memory>

template <class T, typename... Args>
class Singleton {
protected:
	struct guard{};
	Singleton() = default;
	static std::shared_ptr<T> _instance;
public:
	Singleton(Singleton const &) = delete;
	Singleton& operator=(Singleton const &) = delete;

	static T& instance(Args... args);
	static T* pinstance(Args... args);
	virtual ~Singleton() = default;
};

template <class T, typename ... Args>
std::shared_ptr<T> Singleton<T, Args...>::_instance{ nullptr };

template <class T, typename ... Args>
T& Singleton<T, Args...>::instance(Args... args) {
	if (_instance == nullptr)
		_instance = std::shared_ptr<T>(new T(guard{}, args...));
	return *_instance;
}

template <class T, typename ... Args>
T* Singleton<T, Args...>::pinstance(Args... args) {
	if (_instance == nullptr)
		_instance = std::shared_ptr<T>(new T(guard{}, args...));
	return _instance.get();
}

#endif //_SINGLETON_H_
