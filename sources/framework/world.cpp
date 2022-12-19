#include "world.hpp"

void World::Kill(Actor* actor){
	actor->OnKill();
	m_PendingActorsKill.Add(actor);
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
	for (Actor* actor : m_PendingActorsKill)
		m_Actors.UnorderedRemove(actor);
	m_PendingActorsKill.Clear();

	for (ActorComponent* component : m_PendingComponentsRemove)
		m_Components.UnorderedRemove(component);

	m_Actors.Add(Move(m_PendingActorsSpawn));
	m_Components.Add(Move(m_PendingComponentsAdd));
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
