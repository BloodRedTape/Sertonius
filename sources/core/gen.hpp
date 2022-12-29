#pragma once

#define GENERATE_HANDLE_CLASS(ClassName, HandleType, InvalidValue) \
class ClassName { \
private: \
	HandleType m_Handle = InvalidValue; \
public: \
	ClassName() = default; \
 \
	ClassName(const ClassName &) = default; \
 \
	ClassName(HandleType handle): \
		m_Handle(handle) \
	{} \
 \
	ClassName &operator=(const ClassName &) = default; \
 \
	operator HandleType()const{ \
		return m_Handle; \
	} \
 \
	operator bool()const{ \
		return m_Handle != HandleType(InvalidValue); \
	} \
 \
	bool operator==(const ClassName& other)const { \
		return m_Handle == other.m_Handle; \
	} \
 \
	bool operator!=(const ClassName& other)const { \
		return !(*this == other); \
	} \
\
	bool operator!()const { \
		return m_Handle == HandleType(InvalidValue); \
	} \
};
