#include "engine/game_engine.hpp"

GameEngine::GameEngine(Vector2s size, GameMode* game_mode):
	Engine(size, game_mode)
{
	m_Swapchain.OnRecreate.Add({ this, &GameEngine::OnSwapchainRecreated });
}

const Framebuffer* GameEngine::OutputFramebuffer(){
	return PresentTarget();
}

void GameEngine::OnSwapchainRecreated(FramebufferChain* chain){
	m_Renderer3D.OnFramebufferRecreate(chain->PresentTarget()->Size());
}

