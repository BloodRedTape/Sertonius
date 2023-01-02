#pragma once

#include "core/transform.hpp"
#include <core/list.hpp>
#include "components/mesh_component.hpp"
#include "components/camera_component.hpp"
#include "components/point_light_component.hpp"

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

struct RenderPointLight : PointLight {
	Vector3f Position;

	RenderPointLight(const PointLightComponent* light);
};

struct Scene {
	List<RenderMesh> Meshes;
	List<RenderPointLight> PointLights;
	RenderCamera Camera;
};