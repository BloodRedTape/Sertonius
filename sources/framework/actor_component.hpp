#pragma once

#include "framework/object.hpp"

class Actor;

class ActorComponent : public Object {
private:
	WeakObjectPtr<Object> m_OwningActor;

	friend class Actor;
public:
	virtual void Tick(float dt);

	Actor* Owner();

	const Actor* Owner()const;
};

template<typename ComponentType>
struct WeakCompPtr : public WeakObjectPtr<ComponentType>{
	static_assert(IsBaseOf<ActorComponent, ComponentType>::Value, "ComponentType should derive from ActorComponent");

	template<typename OtherComponentType, typename = EnableIfType<IsBaseOf<ComponentType, OtherComponentType>::Value, void>>
	WeakCompPtr(WeakCompPtr<OtherComponentType> ptr):
		WeakObjectPtr(ptr.Pin())
	{}

	WeakCompPtr(ComponentType *component):
		WeakObjectPtr(component)
	{}

	operator ComponentType*() {
		SX_ASSERT(IsAlive());
		return Pin();
	}

	ComponentType &operator*() {
		SX_ASSERT(IsAlive());
		return *Pin();
	}
	ComponentType *operator->() {
		SX_ASSERT(IsAlive());
		return Pin();
	}
};