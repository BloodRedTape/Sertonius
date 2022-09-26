#include "world.hpp"

void World::Kill(Actor* actor){
	m_PendingKill.Add(actor);
}

bool World::Kill(WeakActorPtr<Actor> ptr){
	if (ptr.IsAlive())
		return (Kill(ptr.Pin()), true);
	return false;
}

void World::Tick(float dt) {
	m_Actors.ForEach([dt](Actor *actor) {
		actor->Tick(dt);
	});

	PostTick();
}

void World::PostTick(){
	for (Actor* actor : m_PendingKill)
		m_Actors.UnorderedRemove(actor);
	m_PendingKill.Clear();

	for (Actor& actor : m_PendingSpawn)
		actor.m_OwningWorld = this;

	m_Actors.Add(Move(m_PendingSpawn));
}
