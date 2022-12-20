#include "actor.hpp"
#include "world.hpp"
    
void Actor::OnSpawn(){

}

void Actor::Tick(float dt){ 
    //no-op
}

void Actor::OnKill(){
    m_PendingKill = true;
    if (HasParent()) {
        bool detached = Parent()->DetachChild(this);
        SX_ASSERT(detached);
    }

    for (WeakObjectPtr<Object> child : m_Children)
        OwningWorld()->Kill(static_cast<Actor*>(child.Pin()));
}

bool Actor::IsSpawned() const{
    return m_OwningWorld != nullptr;
}

Actor::Actor(Actor&& other)noexcept:
    Transform(other),
    Object(Move(other)),
    m_Components(Move(other.m_Components)),
    m_OwningWorld(other.m_OwningWorld),
    m_PendingComponentsAdd(Move(other.m_PendingComponentsAdd)),
    m_PendingKill(other.m_PendingKill),
    m_Parent(Move(other.m_Parent)),
    m_Children(Move(other.m_Children))
{
    other.m_OwningWorld = nullptr;
    other.m_PendingKill = false;
    other.m_Parent = {};
}

Actor::~Actor() {
    if (!IsSpawned()) {
        SX_ASSERT(!m_Components.Size());
        return;
    }
    SX_ASSERT(IsPendingKill());
    SX_ASSERT(!m_Children.Size());
    
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
    m_PendingKill = other.m_PendingKill;
    other.m_PendingKill = false;
    m_Parent = Move(other.m_Parent);
    other.m_Parent = {};
    m_Children = Move(other.m_Children);
    Position = other.Position;
    Rotation = other.Rotation;
    other.Position = {};
    other.Rotation = {};
    return *this;
}
Matrix4f Actor::GlobalTransform()const {
    Matrix4f local_transform_matrix = LocalTransform().MakeTransformMatrix();

    if (HasParent())
        return Parent()->GlobalTransform() * local_transform_matrix;

    return local_transform_matrix;
}

const Transform &Actor::LocalTransform()const {
    return *this;
}

void Actor::AttachChild(Actor* child) {
    if (!child)
        return;
    if (child->HasParent())
        child->Parent()->DetachChild(child);

    child->m_Parent = this;
    m_Children.Add(child);
}

bool Actor::DetachChild(Actor* target) {
    if (!target)
        return false;

    auto it = m_Children.FindByPredicate([target](WeakObjectPtr<Object> child) {
        return static_cast<Actor*>(child.Pin()) == target;
    });

    if (it == m_Children.end())
        return false;
    
    return (m_Children.UnorderedRemove(it), true);
}
