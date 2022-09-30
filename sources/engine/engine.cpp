#include "engine.hpp"
#include <core/os/clock.hpp>
#include <graphics/api/gpu.hpp>
#include "components/mesh_component.hpp"
#include "imgui/backend.hpp"

Engine::Engine(Vector2s size, GameMode *game_mode):
	m_Window(size.x, size.y, "Sertonius"),
	m_GameMode(game_mode)
{
	m_Window.SetEventsHandler({ this, &Engine::OnEvent });
}

void Engine::Run(){
	StraitXError = new StringWriterCombiner(StraitXError, &m_LogWriter);
	StraitXOut = new StringWriterCombiner(StraitXOut, &m_LogWriter);
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
			OnImGui();

			m_Swapchain.AcquireNext(&acq);

			m_CmdBuffer->Begin();
			{
				m_Renderer3D.CmdRender(m_CmdBuffer.Get(), OutputFramebuffer(), m_World.BuildScene());
				m_ImGuiBackend.CmdRenderFrame(m_CmdBuffer.Get(), PresentTarget());
			}
			m_CmdBuffer->End();

			GPU::Execute(m_CmdBuffer.Get(), acq, pst, fence);
			fence.WaitAndReset();
			PostRender();
			m_Swapchain.PresentCurrent(&pst);
		}

		m_Window.DispatchEvents();
	}
}

void Engine::OnEvent(const Event& e){
	if (e.Type == EventType::WindowClose)
		return m_Window.Close();
	if (e.Type == EventType::FocusIn)
		m_IsFocused = true;
	if (e.Type == EventType::FocusOut)
		m_IsFocused = false;
	
	m_ImGuiBackend.HandleEvent(e);
}

const Framebuffer* Engine::PresentTarget(){
	return m_Swapchain.CurrentFramebuffer();
}

void Engine::OnImGui() {

}

void Engine::PostRender() {

}
