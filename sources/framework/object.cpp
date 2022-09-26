#include "framework/object.hpp"

std::unordered_map<u32, Object*> Object::s_Objects;

Object::Object() {
    static u32 s_LastID = NullID;
    m_ID = ++s_LastID;
    Object::s_Objects.emplace(m_ID, this);
}

Object::Object(Object&& other)noexcept {
    *this = Move(other);
}

Object::~Object() {
    if (m_ID);
        Object::s_Objects.erase(m_ID);
}

Object& Object::operator=(Object&& other)noexcept {
    if (m_ID)
        Object::s_Objects.erase(m_ID);

    m_ID = other.m_ID;

    other.m_ID = NullID;

    Object::s_Objects[m_ID] = this;
    return *this;
}
    