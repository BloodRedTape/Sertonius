#pragma once

#include "framework/actor.hpp"

template<typename ActorType>
class WeakActorPtr {
    static_assert(IsBaseOf<Actor, ActorType>::Value, "");
private:
    u32 m_ActorID = Actor::NullID;
public:
    WeakActorPtr() = default;
        
    template<typename OtherActorType, typename = EnableIfType<IsBaseOf<ActorType, OtherActorType>::Value, void>>
    WeakActorPtr(WeakActorPtr<OtherActorType> other_ptr) :
        m_ActorID(other_ptr.m_ActorID)
    {}

    WeakActorPtr(Actor* actor):
        m_ActorID(actor->m_ID)
    {}

    ActorType *Pin()const{
        if (!IsAlive())
            return nullptr;
        return static_cast<ActorType*>(Actor::s_Actors[m_ActorID]);
    }

    bool IsAlive()const {
        return m_ActorID != Actor::NullID && Actor::s_Actors.find(m_ActorID) != Actor::s_Actors.end();
    }
};
