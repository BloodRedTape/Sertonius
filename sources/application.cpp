#include "application.hpp"
#include <core/os/clock.hpp>
#include "components/mesh_component.hpp"

Application::Application(GameMode *game_mode):
	m_GameMode(game_mode)
{
	m_Window.SetEventsHandler({ this, &Application::OnEvent });
}

void Application::Run(){
	m_GameMode->InitWorld(m_World);


	Clock cl;
	while (m_Window.IsOpen()) {
		float dt = cl.Restart().AsSeconds();

		m_World.Tick(dt);

		if(m_IsFocused)
			m_Renderer.Render(m_World.BuildScene());
		m_Window.DispatchEvents();
	}
}

void Application::OnEvent(const Event& e){
	if (e.Type == EventType::WindowClose)
		m_Window.Close();
	if (e.Type == EventType::FocusIn)
		m_IsFocused = true;
	if (e.Type == EventType::FocusOut)
		m_IsFocused = false;
}

