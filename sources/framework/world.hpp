#pragma once

#include <core/polymorph_list.hpp>
#include <core/list.hpp>
#include "framework/weak_actor_ptr.hpp"

class World {
private:
    PolymorphList<Actor> m_Actors;
    PolymorphList<Actor> m_PendingSpawn;
    List<Actor*> m_PendingKill;
public:
    template<typename ActorType>
    WeakActorPtr<ActorType> Spawn(ActorType&& actor) {
        WeakActorPtr<ActorType> ptr(&actor);
        m_PendingSpawn.Add(Move(actor));
        return ptr;
    }

    bool Kill(WeakActorPtr<Actor> ptr);

    void Kill(Actor* actor);

    void Tick(float dt);
private:
    void PostTick();
};