#include "sertonius/sertonius_game_mode.hpp"
#include "sertonius/player.hpp"
#include "sertonius/mesh_actor.hpp"
#include "render/mesh.hpp"

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
		m_Time = dt;

		Actor* Target = Owner();
		
		Target->Position = { cos(m_Time), sin(m_Time) };
		Target->Position *= m_Radius;
	}
};

WeakActorPtr<Pawn> SertoniusGameMode::InitWorld(World& world) {
	Mesh mesh({
			Vertex{{ 0.5f, 0.f, 0.5f}, {}, {1.f, 0.f, 0.f}},
			Vertex{{-0.5f, 0.f, 0.5f}, {}, {0.f, 1.f, 0.f}},
			Vertex{{-0.5f, 0.f,-0.5f}, {}, {0.f, 0.f, 1.f}},
		},
		{ 0, 1, 2 },
		AABB3f({}, {}),
		"Mesh"
	);
	world.Spawn<MeshActor>(Mesh::LoadFromFile("content/meshes/axis.fbx"));

	WeakActorPtr<MeshActor> monkey = world.Spawn(MeshActor(Mesh::LoadFromFile("content/meshes/monkey.fbx")));
	monkey.Pin()->AddComponent<SpinningMovementCompennt>(2.f);

	world.Spawn(
		MeshActor(Move(mesh))
	);

	return world.Spawn(Player());
}
