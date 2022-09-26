#pragma once

#include <unordered_map>
#include <core/templates.hpp>
#include <core/move.hpp>

class Object {
public:
    static constexpr u32 NullID = 0;
private:
    static std::unordered_map<u32, Object*> s_Objects;
private:
    u32 m_ID = NullID;
private:
    template<typename Type>
    friend class WeakObjectPtr;
public:
    Object();

    Object(const Object&) = delete;

    Object(Object&& other)noexcept;

    virtual ~Object();

    Object& operator=(Object&& other)noexcept;

    Object& operator=(const Object&) = delete;
    
    template<typename ObjectType, typename = EnableIfType<IsBaseOf<Object, ObjectType>::Value, void>>
    bool IsA()const {
        return dynamic_cast<const ObjectType*>(this);
    }
};

template<typename ObjectType>
class WeakObjectPtr {
    static_assert(IsBaseOf<Object, ObjectType>::Value, "ObjectType should derive from Object");
private:
    u32 m_ObjectID = Object::NullID;
public:
    WeakObjectPtr() = default;
        
    template<typename OtherObjectType, typename = EnableIfType<IsBaseOf<ObjectType, OtherObjectType>::Value, void>>
    WeakObjectPtr(WeakObjectPtr<OtherObjectType> other_ptr) :
        m_ObjectID(other_ptr.m_ObjectID)
    {}

    WeakObjectPtr(Object* object):
        m_ObjectID(object ? object->m_ID : Object::NullID)
    {}

    ObjectType *Pin()const{
        if (!IsAlive())
            return nullptr;
        return static_cast<ObjectType*>(Object::s_Objects[m_ObjectID]);
    }

    bool IsAlive()const {
        return m_ObjectID != Object::NullID && Object::s_Objects.find(m_ObjectID) != Object::s_Objects.end();
    }
};
