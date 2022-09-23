#include "application.hpp"

Application::Application(){
	m_Window.SetEventsHandler({ this, &Application::OnEvent });
}

void Application::Run(){

	while (m_Window.IsOpen()) {
		if(m_IsFocused)
			m_Renderer.Render(/*m_Scene*/);
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

