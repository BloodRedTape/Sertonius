#pragma once

#include "core/transform.hpp"
#include <core/list.hpp>
#include "components/mesh_component.hpp"
#include "components/camera_component.hpp"

struct RenderMesh{
	Matrix4f Transform;
	MeshHandle Mesh;

	RenderMesh(const MeshComponent* mesh);
};

struct RenderCamera : Camera{
	Vector3f Position;
	Vector3f Rotation;

	RenderCamera() = default;

	RenderCamera(const CameraComponent* camera);

	Matrix4f MakeViewMatrix()const;
};

struct Scene {
	List<RenderMesh> Meshes;
	RenderCamera Camera;
};