#include "actor.hpp"
#include "world.hpp"
    
void Actor::Tick(float dt){ 
    //no-op
}

bool Actor::IsInWorld() const{
    return m_OwningWorld != nullptr;
}

Actor::Actor(Actor&& other)noexcept:
    Object(Move(other)),
    m_Components(Move(other.m_Components)),
    m_OwningWorld(other.m_OwningWorld)
{
    other.m_OwningWorld = nullptr;
}

Actor::~Actor() {
    if (!IsInWorld()) {
        SX_ASSERT(!m_Components.Size());
        return;
    }
    
    for (WeakCompPtr<ActorComponent> comp : m_Components) {
        SX_ASSERT(comp.IsAlive());
        m_OwningWorld->RemoveComponent(comp.Pin());
    }
}

Actor& Actor::operator=(Actor&& other)noexcept {
    Object::operator=(Move(other));

    m_Components = Move(other.m_Components);
    m_OwningWorld = other.m_OwningWorld;
    other.m_OwningWorld = nullptr;
    return *this;
}
