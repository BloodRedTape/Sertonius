#pragma once

#include "framework/game_mode.hpp"

class SertoniusGameMode: public GameMode{
public:
	void InitWorld(World& world)override;
};