#pragma once

#include "framework/world.hpp"

class GameMode{
public:
	virtual WeakActorPtr<Actor> InitWorld(World& world) = 0;
};