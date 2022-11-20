#pragma once

#include <core/os/mouse.hpp>
#include <core/os/keyboard.hpp>
#include "framework/actor.hpp"

class Pawn : public Actor {
public:
	virtual void OnMouseMove(Vector2s offset);

	virtual void OnMouseButtonPress(Mouse::Button button, Vector2s position);

	virtual void OnMouseButtonRelease(Mouse::Button button, Vector2s position);
};