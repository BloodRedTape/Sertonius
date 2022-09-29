#include "application.hpp"
#include <core/os/clock.hpp>
#include <graphics/api/gpu.hpp>
#include "components/mesh_component.hpp"
#include "imgui/backend.hpp"

Application::Application(GameMode *game_mode):
	m_GameMode(game_mode)
{
	m_Window.SetEventsHandler({ this, &Application::OnEvent });
	m_Swapchain.OnRecreate.Add({ &m_Renderer, &Renderer::OnSwapchainRecreate });
}

void Application::Run(){
	m_GameMode->InitWorld(m_World);

	Semaphore acq, pst;
	Fence fence;

	UniquePtr<CommandPool> m_Pool(
		CommandPool::Create()
	);
	UniquePtr<CommandBuffer, CommandBufferDeleter> m_CmdBuffer{ m_Pool->Alloc(), {m_Pool.Get()} };

	Clock cl;
	while (m_Window.IsOpen()) {
		float dt = cl.Restart().AsSeconds();
		
		if (m_IsFocused) {
			m_ImGuiBackend.NewFrame(dt, Mouse::RelativePosition(m_Window), m_Window.Size());

			m_World.Tick(dt);
			m_Swapchain.AcquireNext(&acq);

			m_CmdBuffer->Begin();
			{
				m_Renderer.CmdRender(m_CmdBuffer.Get(), m_Swapchain.CurrentFramebuffer(), m_World.BuildScene());
				m_ImGuiBackend.CmdRenderFrame(m_CmdBuffer.Get(), m_Swapchain.CurrentFramebuffer());
			}
			m_CmdBuffer->End();

			GPU::Execute(m_CmdBuffer.Get(), acq, pst, fence);
			m_Swapchain.PresentCurrent(&pst);

			fence.WaitAndReset();
		}

		m_Window.DispatchEvents();
	}
}

void Application::OnEvent(const Event& e){
	if (e.Type == EventType::WindowClose)
		return m_Window.Close();
	if (e.Type == EventType::FocusIn)
		m_IsFocused = true;
	if (e.Type == EventType::FocusOut)
		m_IsFocused = false;
	
	m_ImGuiBackend.HandleEvent(e);
}

void Application::OnImGui(){

}

