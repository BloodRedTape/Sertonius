#pragma once

#include <core/os/window.hpp>
#include "render/renderer.hpp"
#include "framework/world.hpp"
#include "framework/game_mode.hpp"

class Application {
private:
	Window m_Window{ 1280, 720, "Sertonius" };
	Renderer m_Renderer{m_Window};

	bool m_IsFocused = true;
	World m_World;
	GameMode* m_GameMode;
public:

	Application(GameMode *game_mode);

	void Run();

	void OnEvent(const Event& e);
	
	void OnImGui();
};