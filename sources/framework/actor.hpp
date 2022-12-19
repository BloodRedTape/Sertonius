#pragma once


#include <core/list.hpp>
#include <core/math/vector3.hpp>
#include <core/polymorph_list.hpp>
#include "core/transform.hpp"
#include "framework/object.hpp"
#include "framework/actor_component.hpp"

class World;

class Actor: public Object, public Transform{
private:
    World* m_OwningWorld = nullptr;
    List<WeakCompPtr<ActorComponent>> m_Components;
    PolymorphList<ActorComponent> m_PendingComponentsAdd;
private:
    friend class World;
public:
    Actor() = default;

    Actor(Actor&& other)noexcept;

    virtual ~Actor();

    Actor& operator=(Actor&& other)noexcept;

    virtual void OnSpawn();
    
    virtual void Tick(float dt);

    virtual void OnKill();

    bool IsSpawned()const;

    World* OwningWorld() { return m_OwningWorld;  }

    template<typename ComponentType>
    WeakCompPtr<ComponentType> AddComponent(ComponentType&& component) {
        WeakCompPtr<ComponentType> ptr(&component);
        component.m_OwningActor = this;
        if (IsSpawned()) {
            m_OwningWorld->AddComponent(Move(component));
        } else {
            m_PendingComponentsAdd.Add(Move(component));
        }
        return ptr;
    }

    Transform GlobalTransform()const;

    const Transform &LocalTransform()const;
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

#include "world.hpp"
