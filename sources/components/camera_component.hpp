#pragma once

#include "framework/actor_component.hpp"
#include "render/camera.hpp"

class CameraComponent: public ActorComponent, public Camera{
public:
	CameraComponent(Camera camera):
		Camera(camera)
	{}

	Matrix4f MakeViewMatrix()const;
};