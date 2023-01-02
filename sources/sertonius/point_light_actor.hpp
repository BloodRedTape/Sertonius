#pragma once

#include "components/point_light_component.hpp"

class PointLightActor : public Actor {
	WeakCompPtr<PointLightComponent> m_PointLightComponent{ nullptr };
public:
	PointLightActor(PointLight point_light) {
		m_PointLightComponent = AddComponent<PointLightComponent>(point_light);
	}
};