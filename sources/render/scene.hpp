#pragma once

#include "core/transform.hpp"
#include <core/list.hpp>
#include "components/mesh_component.hpp"
#include "components/camera_component.hpp"

struct RenderMesh{
	Matrix4f Transform;
	const class Mesh* Mesh;

	RenderMesh(const MeshComponent* mesh);
};

struct RenderCamera : Camera{
	Matrix4f Transform;

	RenderCamera() = default;

	RenderCamera(const CameraComponent* camera);

	Matrix4f MakeViewMatrix()const;
};

struct Scene {
	List<RenderMesh> Meshes;
	RenderCamera Camera;
};