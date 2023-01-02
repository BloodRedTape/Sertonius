#include "scene.hpp"
#include <core/math/transform.hpp>
#include "framework/actor.hpp"

RenderMesh::RenderMesh(const MeshComponent* mesh) :
	Transform(mesh->Owner()->GlobalTransform()),
	Mesh(mesh->Mesh)
{}

RenderCamera::RenderCamera(const CameraComponent* camera) :
	Camera(*camera)
{
	Rotation = camera->Owner()->Rotation;
	Vector3f _, __;
	camera->Owner()->GlobalTransform().Decompose(Position, _, __);
}

Matrix4f RenderCamera::MakeViewMatrix() const{
	return Math::Rotate<float>(-Math::Rad(Rotation)) * Math::Translate(-Position);
}

RenderPointLight::RenderPointLight(const PointLightComponent* light):
	PointLight(light->PointLight)
{
	Vector3f _, __;
	light->Owner()->GlobalTransform().Decompose(Position, _, __);
}
