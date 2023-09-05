#pragma once
template<typename T>
class Singleton
{
protected:
	Singleton() {}
	virtual ~Singleton() {}
	Singleton(const Singleton &instance) {}
	Singleton &operator=(const Singleton &r) {}
public:
	static T *Get() {
		static T instance;
		return &instance;
	}
};