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

Semaphore acq, pst;
Fence fence;

void Renderer::Render(const Scene &scene){
	OnImGui();

	m_RenderTargets.AcquireNext(&acq);
	m_CmdBuffer->Begin();
	{
		m_GeometryPass.CmdRender(m_CmdBuffer.Get(), scene);
		m_CompositePass.CmdRender(m_CmdBuffer.Get());

		//m_CmdBuffer->ChangeLayout(m_RenderTargets.Normal.Get(), TextureLayout::ShaderReadOnlyOptimal);
		Pointer()->CmdRenderFrame(m_CmdBuffer.Get(), m_RenderTargets.CurrentFramebuffer());
		//m_RenderTargets.Depth->ChangeLayout(TextureLayout::DepthStencilAttachmentOptimal);
	}
	m_CmdBuffer->End();

	GPU::Execute(m_CmdBuffer.Get(), acq, pst, fence);
	fence.WaitAndReset();
	m_RenderTargets.PresentCurrent(&pst);
}

void Renderer::OnImGui(){
	const ImVec2 rt_size = { 160, 90};
	ImGui::Begin("Renderer");

	ImGui::Button("Button");
	ImGui::Text("Test");
	
	ImGui::Image(m_RenderTargets.Albedo.Get(), rt_size);
	ImGui::Image(m_RenderTargets.Normal.Get(), rt_size);
	ImGui::Image(m_RenderTargets.Position.Get(), rt_size);
	ImGui::Image(m_RenderTargets.Depth.Get(), rt_size);

	ImGui::End();
}
