#pragma once

#include "framework/actor_component.hpp"
#include "framework/assets_manager.hpp"

class MeshComponent : public ActorComponent{
public:
	MeshHandle Mesh;
public:
	MeshComponent(MeshHandle mesh);
};
