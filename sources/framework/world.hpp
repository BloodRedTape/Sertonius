#pragma once

#include <core/polymorph_list.hpp>
#include <core/list.hpp>
#include "framework/actor.hpp"

class World {
private:
    PolymorphList<Actor> m_Actors;
    PolymorphList<Actor> m_PendingActorsSpawn;
    List<Actor*> m_PendingActorsKill;

    PolymorphList<ActorComponent> m_Components;
    PolymorphList<ActorComponent> m_PendingComponentsAdd;
    List<ActorComponent*> m_PendingComponentsRemove;
public:
    template<typename ActorType>
    WeakActorPtr<ActorType> Spawn(ActorType&& actor) {
        WeakActorPtr<ActorType> ptr(&actor);
        actor.m_OwningWorld = this;
        m_PendingActorsSpawn.Add(Move(actor));
        return ptr;
    }

    bool Kill(WeakActorPtr<Actor> ptr);

    void Kill(Actor* actor);

private:
    template<typename ComponentType>
    WeakCompPtr<ComponentType> AddComponent(ComponentType&& component) {
        WeakCompPtr<ComponentType> ptr(&component);
        m_PendingComponentsAdd.Add(Move(component));
        return ptr;
    }

    bool RemoveComponent(WeakCompPtr<ActorComponent> component);

    void RemoveComponent(ActorComponent* component);

    friend class Actor;
public:
    void Tick(float dt);
private:
    void PostTick();
};
