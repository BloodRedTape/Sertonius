#pragma once

#include <unordered_map>
#include <core/templates.hpp>
#include <core/move.hpp>

class World;

class Actor {
public:
    static constexpr u32 NullID = 0;
private:
    static std::unordered_map<u32, Actor*> s_Actors;
private:
    World* m_OwningWorld = nullptr;
    u32 m_ID = NullID;
private:
    template<typename Type>
    friend class WeakActorPtr;
    friend class World;
public:
    Actor();

    Actor(Actor&& other)noexcept;

    virtual ~Actor();

    Actor& operator=(Actor&& other)noexcept;
    
    template<typename ActorType, typename = EnableIfType<IsBaseOf<Actor, ActorType>::Value, void>>
    bool IsA()const {
        return dynamic_cast<const ActorType*>(this);
    }

    virtual void Tick(float dt);
};
