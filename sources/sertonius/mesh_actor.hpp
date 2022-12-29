#pragma once

#include "components/mesh_component.hpp"

class MeshActor : public Actor {
	WeakCompPtr<MeshComponent> m_MeshComponent{ nullptr };
public:
	MeshActor(MeshHandle mesh) {
		 m_MeshComponent = AddComponent<MeshComponent>(mesh);
	}
};