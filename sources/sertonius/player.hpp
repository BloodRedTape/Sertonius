#pragma once

#include "components/camera_component.hpp"
#include "framework/actor.hpp"

class Player : public Actor {
public:
	Player();

	void Tick(float dt)override;
};
