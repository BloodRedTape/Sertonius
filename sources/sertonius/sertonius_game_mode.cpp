#include "sertonius/sertonius_game_mode.hpp"
#include "sertonius/player.hpp"
#include "sertonius/mesh_actor.hpp"
#include "render/mesh.hpp"

void SertoniusGameMode::InitWorld(World& world) {
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
	//world.Spawn(MeshActor(Mesh::LoadFromFile("content/meshes/monkey.fbx")));
	world.Spawn(
		MeshActor(Move(mesh))
	);

	world.Spawn(Player());
}
