#include "render/renderer.hpp"
#include <graphics/api/gpu.hpp>
#include <core/unique_ptr.hpp>

Renderer::Renderer(const Window &window):
	m_Window(window),
	m_RenderTargets(window)
{
	Construct(m_RenderTargets.Pass());
}

Renderer::~Renderer() {
	Destruct();
}

Semaphore acq, gui, pst;
Fence fence;

//int s = (fence.Signal(), 1);

void Renderer::Render(const Scene &scene){
	m_RenderTargets.AcquireNext(&acq);

	m_CmdBuffer->Begin();
	m_GeometryPass.CmdRender(m_CmdBuffer.Get(), scene);
	m_CompositePass.CmdRender(m_CmdBuffer.Get());
	m_CmdBuffer->End();


	GPU::Execute(m_CmdBuffer.Get(), acq, gui, fence);
	fence.WaitAndReset();
	OnImGui();
	m_RenderTargets.Depth->ChangeLayout(TextureLayout::ShaderReadOnlyOptimal);
	Pointer()->RenderFrame(m_RenderTargets.CurrentFramebuffer(), &gui, &pst);
	m_RenderTargets.Depth->ChangeLayout(TextureLayout::DepthStencilAttachmentOptimal);
	m_RenderTargets.PresentCurrent(&pst);
}

void Renderer::OnImGui(){
	const ImVec2 rt_size = { 160, 90};
	ImGui::Begin("Renderer");
	
	ImGui::Image(m_RenderTargets.Albedo.Get(), rt_size);
	ImGui::Image(m_RenderTargets.Normal.Get(), rt_size);
	ImGui::Image(m_RenderTargets.Position.Get(), rt_size);
	ImGui::Image(m_RenderTargets.Depth.Get(), rt_size);

	ImGui::End();
}
