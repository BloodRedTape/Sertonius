#include "components/camera_component.hpp"
#include "framework/actor.hpp"

Matrix4f CameraComponent::MakeViewMatrix() const{
	return Camera::MakeViewMatrix(Owner()->Position, Owner()->Rotation);
}

