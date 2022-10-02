#pragma once

#include "render/renderer3d.hpp"
#include "framework/world.hpp"
#include "framework/game_mode.hpp"
#include <graphics/api/swapchain.hpp>
#include <imgui/backend.hpp>
#include "core/log.hpp"

class Engine {
protected:
	Log m_Log;
	LogWriter m_LogWriter{ m_Log };
	LogInitializer m_LogInitializer{ m_LogWriter };

	Window m_Window;
	FramebufferChain m_Swapchain{&m_Window};
	Renderer3D m_Renderer3D{m_Swapchain};
	ImGuiBackend m_ImGuiBackend{ m_Swapchain.Pass() };

	bool m_IsFocused = true;
	World m_World;
	GameMode* m_GameMode;
public:

	Engine(Vector2s size, GameMode *game_mode);

	void Run();

	void OnEvent(const Event& e);
	
	virtual const Framebuffer* OutputFramebuffer() = 0;

	const Framebuffer* PresentTarget();

	virtual void OnImGui();

	virtual void PostRender();
};