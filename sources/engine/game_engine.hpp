#pragma once

#include "engine/engine.hpp"

class GameEngine : public Engine {
public:
	GameEngine(Vector2s size, GameMode* game_mode);

	const Framebuffer* OutputFramebuffer();

	void OnSwapchainRecreated(FramebufferChain* chain);
};