#pragma once

#include "components/mesh_component.hpp"

class MeshActor : public Actor {
public:
	MeshActor(Mesh mesh) {
		AddComponent(MeshComponent(Move(mesh)));
	}
};