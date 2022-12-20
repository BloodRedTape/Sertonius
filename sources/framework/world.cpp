#include "world.hpp"

World::~World() {
	m_Actors.ForEach([this](Actor *actor) {
		Kill(actor);
	});

	KillPendingActorsAndComponents();
}

void World::Kill(Actor* actor){
	if (actor->IsPendingKill())
		return;
	m_PendingActorsKill.Add(actor);
	actor->OnKill();
}

bool World::Kill(WeakActorPtr<Actor> ptr){
	if (ptr.IsAlive())
		return (Kill(ptr.Pin()), true);
	return false;
}

bool World::RemoveComponent(WeakCompPtr<ActorComponent> component){
	if(component.IsAlive())
		return (m_PendingComponentsRemove.Add(component.Pin()), true);
	return false;
}

void World::RemoveComponent(ActorComponent* component){
	m_PendingComponentsRemove.Add(component);
}

void World::Tick(float dt) {
	m_Actors.ForEach([dt](Actor *actor) {
		actor->Tick(dt);
	});
	m_Components.ForEach([dt](ActorComponent *component) {
		component->Tick(dt);
	});

	PostTick();
}

void World::PostTick(){
	KillPendingActorsAndComponents();
	SpawnPendingActorsAndComponents();
}

void World::SpawnPendingActorsAndComponents(){	
	m_Actors.Add(Move(m_PendingActorsSpawn));
	m_Components.Add(Move(m_PendingComponentsAdd));
}

void World::KillPendingActorsAndComponents(){
	for (WeakActorPtr<Actor> actor : m_PendingActorsKill)
		m_Actors.UnorderedRemove(actor.Pin());
	m_PendingActorsKill.Clear();

	for (WeakCompPtr<ActorComponent> component : m_PendingComponentsRemove)
		m_Components.UnorderedRemove(component.Pin());
}

Scene World::BuildScene(){
	Scene scene;
	for (const MeshComponent& mesh : m_Components.TypeRange<MeshComponent>())
		scene.Meshes.Add(&mesh);
	
	auto camera_range = m_Components.TypeRange<CameraComponent>();

	CameraComponent* camera = camera_range.Size() ? &camera_range[0] : nullptr;
	scene.Camera = camera ? RenderCamera(camera) : RenderCamera();

	return scene;
}
