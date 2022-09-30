#pragma once

#include "render/renderer.hpp"
#include "framework/world.hpp"
#include "framework/game_mode.hpp"
#include <graphics/api/swapchain.hpp>
#include <imgui/backend.hpp>
#include "core/log.hpp"

class Application {
private:
	Window m_Window{ 1920, 1080, "Sertonius" };
	FramebufferChain m_Swapchain{&m_Window};
	Renderer m_Renderer{m_Swapchain};
	ImGuiBackend m_ImGuiBackend{ m_Swapchain.Pass() };

	Log m_Log;
	LogWriter m_LogWriter{ m_Log };

	bool m_IsFocused = true;
	World m_World;
	GameMode* m_GameMode;
public:

	Application(GameMode *game_mode);

	void Run();

	void OnEvent(const Event& e);
	
	void OnImGui();
};