#include "sertonius/sertonius_game_mode.hpp"
#include "sertonius/player.hpp"
#include "sertonius/mesh_actor.hpp"
#include "render/mesh.hpp"
#include "framework/assets_manager.hpp"
#include <core/print.hpp>

class SpinningMovementCompennt : public ActorComponent {
private:
	float m_Time = 0.f;
	const float m_Radius;
public:
	SpinningMovementCompennt(float radius):
		m_Radius(radius)
	{}

	void Tick(float dt)override {
		ActorComponent::Tick(dt);
		m_Time += dt;


		Actor* Target = Owner();
		
		Target->Position = { cos(m_Time), sin(m_Time), 0};
		Target->Position *= m_Radius;

		Println("position: %", Target->Position);
	}
};

class RolingMovementCompennt : public ActorComponent {
public:
	void Tick(float dt)override {
		ActorComponent::Tick(dt);
		Actor* Target = Owner();

		Target->Rotation.y += dt * 90.f;
		Println("rotation: %", Target->Rotation);
	}
};

WeakActorPtr<Pawn> SertoniusGameMode::InitWorld(World& world) {
	Mesh mesh({
			Vertex{{ 0.5f, 0.f, 0.5f}, {}},
			Vertex{{-0.5f, 0.f, 0.5f}, {}},
			Vertex{{-0.5f, 0.f,-0.5f}, {}},
		},
		{ 0, 1, 2 },
		AABB3f({}, {}),
		"Mesh"
	);
	world.Spawn<MeshActor>(AssetsManager::Get().GetOrLoadMesh("content/meshes/axis.fbx"));
	
	Actor* spinner = world.Spawn(Actor()).Pin();
	spinner->AddComponent<SpinningMovementCompennt>(5.f);
	spinner->AddComponent<RolingMovementCompennt>({});
	
	MeshActor *monkey = world.Spawn(MeshActor(AssetsManager::Get().GetOrLoadMesh("content/meshes/monkey.fbx"))).Pin();
	spinner->AttachChild(monkey);
	monkey->Position.z = 3;

	world.Spawn(
		MeshActor(AssetsManager::Get().Add(Move(mesh)))
	);

	return world.Spawn(Player());
}
