#include "application.hpp"
#include "components/mesh_component.hpp"

Application::Application(GameMode *game_mode):
	m_GameMode(game_mode)
{
	m_Window.SetEventsHandler({ this, &Application::OnEvent });
}

void Application::Run(){
	m_GameMode->InitWorld(m_World);



	while (m_Window.IsOpen()) {

		m_World.Tick(0);

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

