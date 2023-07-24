#pragma once
#include "AtExit.h"

#include <memory>

#define SINGLETON(_Type) \
friend class Singleton<_Type>;\
friend std::unique_ptr<_Type> std::make_unique<_Type>();\
friend std::unique_ptr<_Type>::deleter_type;\
private:\
_Type();\
~_Type()


template<typename T>
class Singleton
{
	friend class std::unique_ptr<T>;
protected:
	Singleton() {}
	virtual ~Singleton() {}

private:
	static std::unique_ptr<T> m_Inst;

public:
	static T* GetInst();
	static void Destroy();
};

template<typename T>
std::unique_ptr<T> Singleton<T>::m_Inst = nullptr;

template<typename T>
inline T* Singleton<T>::GetInst()
{
	if (nullptr == m_Inst)
	{
		m_Inst = std::make_unique<T>();
		AtExit::AddFunc(Destroy);
	}

	return m_Inst.get();
}

template<typename T>
inline void Singleton<T>::Destroy()
{
	m_Inst.reset();
}


