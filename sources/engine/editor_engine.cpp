#include "engine/editor_engine.hpp"

EditorEngine::EditorEngine(Vector2s size, GameMode* game_mode):
	Engine(size, game_mode),
	m_RenderTexture(size, TextureFormat::BGRA8, TextureFormat::Unknown),
	m_LastOutputFramebufferSize(m_RenderTexture.Size())
{}

const Framebuffer* EditorEngine::OutputFramebuffer(){
	return m_RenderTexture.Framebuffer();
}

void EditorEngine::OnImGui(){
	ImGui::DockspaceWindow("DockSpace", PresentTarget()->Size());

	ImGui::Begin("Viewport");
	Vector2s size = { (s32)ImGui::GetContentRegionAvail().x, (s32)ImGui::GetContentRegionAvail().y };
	if (size != m_LastOutputFramebufferSize) {
		m_LastOutputFramebufferSize = size;
		m_PendingResize = size;
	}

	ImGui::Image((ImTextureID)m_RenderTexture.ColorAttachment(), { (float)size.x, (float)size.y });
	ImGui::End();

	ImGui::Begin("Log");
	m_Log.DrawImGuiText();
	ImGui::End();
}

void EditorEngine::PostRender(){
	if (!m_PendingResize.HasValue())return;
	
	Vector2s new_size = m_PendingResize.Value();

	if (!new_size.x && !new_size.y)return;

	m_RenderTexture.Resize(new_size);
	m_Renderer3D.OnFramebufferRecreate(new_size);

	m_PendingResize.Clear();
}

