#pragma once

#include "components/mesh_component.hpp"

class MeshActor : public Actor {
	WeakCompPtr<MeshComponent> m_MeshComponent{ nullptr };
public:
	MeshActor(Mesh mesh) {
		 m_MeshComponent = AddComponent<MeshComponent>(Move(mesh));
	}
};