#pragma once

#include <core/list.hpp>
#include <core/noncopyable.hpp>

template<typename Type>
class ReusableObjectPool: public NonCopyable{
private:
	List<Type*> m_Objects;
	size_t m_Allocated = 0;
public:

	~ReusableObjectPool() {
		for (Type* obj : m_Objects)
			delete obj;
	}
	
	Type* AllocOrReuse() {
		if (m_Objects.Size() == m_Allocated)
			m_Objects.Add(new Type());
		return m_Objects[m_Allocated++];
	}

	void Reset() {
		m_Allocated = 0;
	}
};