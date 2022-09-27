#include "components/camera_component.hpp"

Matrix4f CameraComponent::MakeViewMatrix() const{
	return Matrix4f{ 1.f };
}

