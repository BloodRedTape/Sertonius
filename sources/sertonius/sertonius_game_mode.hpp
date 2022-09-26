#pragma once

#include "framework/game_mode.hpp"

class SertoniusGameMode: public GameMode{
public:
	WeakActorPtr<Actor> InitWorld(World& world);
};