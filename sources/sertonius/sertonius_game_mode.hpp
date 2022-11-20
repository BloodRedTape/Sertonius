#pragma once

#include "framework/game_mode.hpp"

class SertoniusGameMode: public GameMode{
public:
	WeakActorPtr<Pawn> InitWorld(World& world)override;
};