#pragma once

#include <core/os/window.hpp>
#include "render/renderer.hpp"
#include "framework/world.hpp"

class Application {
private:
	Window m_Window{ 1280, 720, "Sertonius" };
	Renderer m_Renderer{m_Window};

	bool m_IsFocused = true;
	World m_World;
public:

	Application();

	void Run();

	void OnEvent(const Event& e);
};