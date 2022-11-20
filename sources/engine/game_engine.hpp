#pragma once

#include "engine/engine.hpp"

class GameEngine : public Engine {
private:
	Vector2s m_LastMousePosition;
public:
	GameEngine(Vector2s size, GameMode* game_mode);

	const Framebuffer* OutputFramebuffer();

	void OnSwapchainRecreated(FramebufferChain* chain);

	void Tick(float dt)override;
private:
	void OnEvent(const Event& e)override;
};