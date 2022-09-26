#include "application.hpp"

Application::Application(){
	m_Window.SetEventsHandler({ this, &Application::OnEvent });
}

void Application::Run(){

	WeakActorPtr<Actor> ptr = m_World.Spawn(Actor());
	if (!ptr.IsAlive()) {
		int n = 0;
	}
	Actor* actor = ptr.Pin();

	WeakCompPtr<ActorComponent> cptr = actor->AddComponent(ActorComponent());

	Mesh mesh({
			Vertex{{ 0.5f, 0.5f, 0.f}},
			Vertex{{-0.5f, 0.5f, 0.f}},
			Vertex{{-0.5f,-0.5f, 0.f}},
		}, 
		{0, 1, 2}, 
		AABB3f({}, {}), 
		"Mesh"
	);

	List<Mesh> meshes;
	meshes.Add(Move(mesh));
	meshes.Add(Mesh::LoadFromFile("content/meshes/monkey.fbx"));

	while (m_Window.IsOpen()) {


		if(m_IsFocused)
			m_Renderer.Render(meshes);
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

