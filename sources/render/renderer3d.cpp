#include "render/renderer3d.hpp"
#include <graphics/api/gpu.hpp>
#include <core/unique_ptr.hpp>

Renderer3D::Renderer3D(const FramebufferChain& chain) :
	m_Pass(chain.Pass()),
	m_RenderTargets(chain.PresentTarget()->Size())
{
}

void Renderer3D::CmdRender(CommandBuffer* cmd_buffer, const Framebuffer* fb, const Scene& scene) {
	OnImGui();

	m_GeometryPass.CmdRender(cmd_buffer, scene);
	m_LightingPass.CmdRender(cmd_buffer, scene);
	m_CompositePass.CmdRender(cmd_buffer, fb);
}

void Renderer3D::OnImGui(){
	const float rt_height = 250;
	const float aspect = m_RenderTargets.Size().x / float(m_RenderTargets.Size().y);
	const ImVec2 rt_size = ImVec2{ rt_height * aspect, rt_height};
	ImGui::Begin("Renderer");
	
	ImGui::Text("FramebufferSize: (%d, %d)", m_RenderTargets.Size().x, m_RenderTargets.Size().y);
	
	ImGui::Image(m_RenderTargets.Albedo.Get(), rt_size);
	ImGui::Image(m_RenderTargets.Normal.Get(), rt_size);
	ImGui::Image(m_RenderTargets.Position.Get(), rt_size);
	ImGui::Image(m_RenderTargets.Depth.Get(), rt_size);
	ImGui::Image(m_RenderTargets.Lighting.Get(), rt_size);

	ImGui::End();
}

void Renderer3D::OnFramebufferRecreate(Vector2s size){
	m_RenderTargets.OnFramebufferRecreate(size);
}
