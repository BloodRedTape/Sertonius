#include "scene.hpp"
#include <core/math/transform.hpp>
#include "framework/actor.hpp"

RenderMesh::RenderMesh(const MeshComponent* mesh) :
	Transform(mesh->Owner()->GlobalTransform()),
	Mesh(mesh)
{}

RenderCamera::RenderCamera(const CameraComponent* camera):
	Transform(camera->Owner()->GlobalTransform()),
	Camera(*camera)
{}

Matrix4f RenderCamera::MakeViewMatrix() const{
	return Transform.GetInverse();
}
