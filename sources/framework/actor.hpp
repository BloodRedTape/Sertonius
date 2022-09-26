#pragma once


#include <core/list.hpp>
#include "framework/object.hpp"
#include "framework/actor_component.hpp"

class World;

class Actor: public Object{
private:
    World* m_OwningWorld = nullptr;
    List<WeakCompPtr<ActorComponent>> m_Components;
private:
    friend class World;
public:
    Actor() = default;

    Actor(Actor&& other)noexcept;

    virtual ~Actor();

    Actor& operator=(Actor&& other)noexcept;
    
    virtual void Tick(float dt);

    bool IsInWorld()const;

    template<typename ComponentType>
    WeakCompPtr<ComponentType> AddComponent(ComponentType&& component) {
        WeakCompPtr<ComponentType> ptr(&component);
        component.m_OwningActor = this;
        m_OwningWorld->AddComponent(Move(component));
        return ptr;
    }
};

template<typename ActorType>
struct WeakActorPtr : public WeakObjectPtr<ActorType>{
	static_assert(IsBaseOf<Actor, ActorType>::Value, "ActorType should derive from Actor");

	WeakActorPtr(ActorType *actor):
		WeakObjectPtr(actor)
	{}

    template<typename OtherActorType, typename = EnableIfType<IsBaseOf<ActorType, OtherActorType>::Value, void>>
	WeakActorPtr(WeakActorPtr<OtherActorType> ptr):
		WeakObjectPtr(ptr.Pin())
	{}
};
