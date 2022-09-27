#pragma once

#include "components/mesh_component.hpp"
#include "components/camera_component.hpp"

struct Scene {
	ConstSpan<MeshComponent> Meshes;
	class CameraComponent* CameraComponent;
};