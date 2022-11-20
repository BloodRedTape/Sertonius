#include "engine/game_engine.hpp"
#include <core/print.hpp>

GameEngine::GameEngine(Vector2s size, GameMode* game_mode):
	Engine(size, game_mode)
{
	m_Swapchain.OnRecreate.Add({ this, &GameEngine::OnSwapchainRecreated });

	m_LastMousePosition = Mouse::RelativePosition(m_Window);
}

const Framebuffer* GameEngine::OutputFramebuffer(){
	return PresentTarget();
}

void GameEngine::OnSwapchainRecreated(FramebufferChain* chain){
	m_Renderer3D.OnFramebufferRecreate(chain->PresentTarget()->Size());
}

void GameEngine::Tick(float dt){
	Engine::Tick(dt);

	Vector2s new_mouse_position = Mouse::RelativePosition(m_Window);
	Vector2s offset = new_mouse_position - m_LastMousePosition;

	Pawn* player = m_Player.Pin();
	if (!player)return;

	if (!(offset.x || offset.y))return;
		
	player->OnMouseMove(offset);
	m_LastMousePosition = new_mouse_position;
}

void GameEngine::OnEvent(const Event& e){
	Engine::OnEvent(e);

	Pawn* player = m_Player.Pin();

	if (!player)return;

	if (e.Type == EventType::MouseButtonPress)
		player->OnMouseButtonPress(e.MouseButtonPress.Button, { e.MouseButtonPress.x, e.MouseButtonPress.y });
	if (e.Type == EventType::MouseButtonRelease)
		player->OnMouseButtonRelease(e.MouseButtonRelease.Button, { e.MouseButtonRelease.x, e.MouseButtonRelease.y });
}

