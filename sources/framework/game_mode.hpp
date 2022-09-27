#pragma once

#include "framework/world.hpp"

class GameMode{
public:
	virtual void InitWorld(World& world) = 0;
};