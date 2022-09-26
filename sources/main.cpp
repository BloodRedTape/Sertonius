#include <core/print.hpp>
#include <core/os/directory.hpp>
#include <graphics/api/gpu.hpp>
#include "application.hpp"
#include "framework/game_mode.hpp"
#include "components/mesh_component.hpp"

class Player : public Actor {
public:

};

class MeshActor : public Actor {
public:
	MeshActor(Mesh mesh) {
		AddComponent(MeshComponent(Move(mesh)));
	}
};

class TestGameMode: public GameMode{
public:
	WeakActorPtr<Actor> InitWorld(World& world) {
		Mesh mesh({
				Vertex{{ 0.5f, 0.5f, 0.f}},
				Vertex{{-0.5f, 0.5f, 0.f}},
				Vertex{{-0.5f,-0.5f, 0.f}},
			}, 
			{0, 1, 2}, 
			AABB3f({}, {}), 
			"Mesh"
		);
		world.Spawn(MeshActor(Mesh::LoadFromFile("content/meshes/monkey.fbx")));
		world.Spawn(MeshActor(Move(mesh)));
		return world.Spawn(Player());
	}
};

int main(){
#ifdef SX_OS_WINDOWS
	Directory::Change("../../../");
#endif
	GPU::ForceInit();

	if (!GPU::IsComputePipelineSupported())
		return (Println("compute is not supported"), EXIT_FAILURE);

	TestGameMode mode;
	Application(&mode).Run();
}
