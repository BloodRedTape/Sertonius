#include "framework/actor_component.hpp"
#include "framework/actor.hpp"

void ActorComponent::Tick(float dt){
	//no-op
}

Actor* ActorComponent::Owner(){
	return dynamic_cast<Actor*>(m_OwningActor.Pin());
}

const Actor* ActorComponent::Owner() const{
	return dynamic_cast<Actor*>(m_OwningActor.Pin());
}
