#include "render/renderer.hpp"
#include <graphics/api/gpu.hpp>
#include <core/unique_ptr.hpp>

Renderer::Renderer(const FramebufferChain& chain) :
	m_Pass(chain.Pass()),
	m_RenderTargets(chain.PresentTarget()->Size())
{
}

void Renderer::CmdRender(CommandBuffer* cmd_buffer, const Framebuffer* fb, const Scene& scene) {
	OnImGui();

	m_GeometryPass.CmdRender(cmd_buffer, scene);
	m_CompositePass.CmdRender(cmd_buffer, fb);
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

void Renderer::OnSwapchainRecreate(FramebufferChain* chain){
	m_RenderTargets.OnRecreate(chain);
}
