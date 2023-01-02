#pragma once

#include "render/lights.hpp"
#include "framework/actor_component.hpp"

class PointLightComponent : public ActorComponent{
public:
	PointLight PointLight;
public:
	PointLightComponent(struct PointLight point_light):
		PointLight(point_light)
	{}
};