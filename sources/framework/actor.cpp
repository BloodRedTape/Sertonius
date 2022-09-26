#include "actor.hpp"

std::unordered_map<u32, Actor*> Actor::s_Actors;

Actor::Actor() {
    static u32 s_LastID = NullID;
    m_ID = ++s_LastID;
    Actor::s_Actors.emplace(m_ID, this);
}

Actor::Actor(Actor&& other)noexcept {
    *this = Move(other);
}

Actor::~Actor() {
    if (m_ID);
    Actor::s_Actors.erase(m_ID);
}

Actor& Actor::operator=(Actor&& other)noexcept {
    if (m_ID)
        s_Actors.erase(m_ID);
    m_ID = other.m_ID;
    other.m_ID = NullID;
    Actor::s_Actors[m_ID] = this;
    return *this;
}
    
void Actor::Tick(float dt){ 
    //no-op
}
