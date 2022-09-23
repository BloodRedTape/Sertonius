#include "application.hpp"

Application::Application(){
	m_Window.SetEventsHandler({ this, &Application::OnEvent });
}

void Application::Run(){

	Mesh mesh({
			Vertex{{ 0.5f, 0.5f, 0.f}},
			Vertex{{-0.5f, 0.5f, 0.f}},
			Vertex{{-0.5f,-0.5f, 0.f}},
		}, 
		{0, 1, 2}, 
		AABB3f({}, {}), 
		"Mesh"
	);

	while (m_Window.IsOpen()) {


		if(m_IsFocused)
			m_Renderer.Render({ &mesh , 1});
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

