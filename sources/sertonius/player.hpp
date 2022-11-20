#pragma once

#include "components/camera_component.hpp"
#include "framework/pawn.hpp"

class Player : public Pawn{
public:
	Player();

	void Tick(float dt)override;

	void OnMouseMove(Vector2s offset)override;

	void OnMouseButtonPress(Mouse::Button button, Vector2s position)override;
};
