#include "sertonius/sertonius_game_mode.hpp"
#include "sertonius/player.hpp"
#include "sertonius/mesh_actor.hpp"
#include "render/mesh.hpp"
#include "framework/assets_manager.hpp"
#include "framework/actor_loader.hpp"
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
	}
};

class RolingMovementCompennt : public ActorComponent {
public:
	void Tick(float dt)override {
		ActorComponent::Tick(dt);
		Actor* Target = Owner();

		Target->Rotation.y += dt * 90.f;
	}
};

WeakActorPtr<Pawn> SertoniusGameMode::InitWorld(World& world) {
	Mesh mesh({
			Vertex{{ 0.5f, 0.0f, 0.5f}, {}},
			Vertex{{ 0.5f, 0.5f,-0.5f}, {}},
			Vertex{{-0.5f, 0.0f,-0.5f}, {}},
		},
		{ 0, 1, 2 },
		AABB3f({}, {}),
		"Mesh"
	);
	
	ActorLoader(&world, "content/meshes/Sponza/Sponza.fbx").Load();
#if 0
	world.Spawn(
		MeshActor(AssetsManager::Add(Move(mesh)))
	);
#endif

	return world.Spawn(Player());
}
