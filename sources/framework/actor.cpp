#include "actor.hpp"
#include "world.hpp"
    
void Actor::OnSpawn(){

}

void Actor::Tick(float dt){ 
    //no-op
}

void Actor::OnKill(){

}

bool Actor::IsSpawned() const{
    return m_OwningWorld != nullptr;
}

Actor::Actor(Actor&& other)noexcept:
    Object(Move(other)),
    m_Components(Move(other.m_Components)),
    m_OwningWorld(other.m_OwningWorld),
    m_PendingComponentsAdd(Move(other.m_PendingComponentsAdd)),
    Position(other.Position),
    Rotation(other.Rotation)
{
    other.m_OwningWorld = nullptr;
}

Actor::~Actor() {
    if (!IsSpawned()) {
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
    m_PendingComponentsAdd = Move(other.m_PendingComponentsAdd);
    Position = other.Position;
    Rotation = other.Rotation;
    other.Position = {};
    other.Rotation = {};
    return *this;
}
