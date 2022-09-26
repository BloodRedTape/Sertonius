#pragma once

#include "framework/actor_component.hpp"
#include "render/mesh.hpp"

class MeshComponent : public ActorComponent, public Mesh {
public:
	MeshComponent(Mesh mesh);
};
