#pragma once

#include "framework/world.hpp"
#include "framework/pawn.hpp"

class GameMode{
public:
	virtual WeakActorPtr<Pawn> InitWorld(World& world) = 0;
};