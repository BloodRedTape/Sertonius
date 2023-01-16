#include "engine/editor_engine.hpp"
#include <core/print.hpp>

EditorEngine::EditorEngine(Vector2s size, GameMode* game_mode):
	Engine(size, game_mode),
	m_RenderTexture(size, TextureFormat::BGRA8, TextureFormat::Unknown),
	m_LastOutputFramebufferSize(m_RenderTexture.Size())
{
	Mouse::SetVisible(false);
}

const Framebuffer* EditorEngine::OutputFramebuffer(){
	return m_RenderTexture.Framebuffer();
}

void EditorEngine::OnImGui(){
	Engine::OnImGui();

	ImGui::DockspaceWindow("DockSpace", PresentTarget()->Size());
	
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });
	ImGui::Begin("Viewport");
	m_Viewport.Min = Vector2s{ (s32)ImGui::GetWindowContentRegionMin().x, (s32)ImGui::GetWindowContentRegionMin().y };
	m_Viewport.Max = m_Viewport.Min + Vector2s{ (s32)ImGui::GetContentRegionAvail().x, (s32)ImGui::GetContentRegionAvail().y };
	if (m_Viewport.Size() != m_LastOutputFramebufferSize) {
		m_LastOutputFramebufferSize = m_Viewport.Size();
		m_PendingResize = m_Viewport.Size();
	}

	ImGui::Image((ImTextureID)m_RenderTexture.ColorAttachment(), { (float)m_Viewport.Size().x, (float)m_Viewport.Size().y });
	ImGui::End();
	ImGui::PopStyleVar();

	ImGui::Begin("Log");
	m_Log.DrawImGuiText();
	ImGui::End();
}

void EditorEngine::Tick(float dt){
	Engine::Tick(dt);
	
	Pawn* player = m_Player.Pin();
	if (!player)
		return;

	Vector2s mouse_position = Mouse::RelativePosition(m_Window);
	Vector2s offset = mouse_position - m_LastMousePosition;

	if (!(offset.x && offset.y))
		return;
	

	m_LastMousePosition = mouse_position;

	if (!m_IsViewportInFocus)
		return;

	player->OnMouseMove(offset);
}

void EditorEngine::PostRender(){
	if (!m_PendingResize.HasValue())return;
	
	Vector2s new_size = m_PendingResize.Value();

	if (!new_size.x && !new_size.y)return;

	m_RenderTexture.Resize(new_size);
	m_Renderer3D.OnFramebufferRecreate(new_size);

	m_PendingResize.Clear();
}

void EditorEngine::OnEvent(const Event& e){
	Engine::OnEvent(e);
	
	Pawn* player = m_Player.Pin();

	if (!player)return;

	if (m_IsViewportInFocus) {

		if (e.Type == EventType::MouseButtonPress) {
			Vector2s point = { e.MouseButtonPress.x, e.MouseButtonPress.y };
			if (m_Viewport.Inside(point))
				player->OnMouseButtonPress(e.MouseButtonPress.Button, point - m_Viewport.Min);
		}
		if (e.Type == EventType::MouseButtonRelease) {
			Vector2s point = { e.MouseButtonRelease.x, e.MouseButtonRelease.y };
			if (m_Viewport.Inside(point))
				player->OnMouseButtonRelease(e.MouseButtonRelease.Button, point - m_Viewport.Min);
		}
	}

	if (e.Type == EventType::KeyPress) {
		if (e.KeyPress.KeyCode == Key::Escape) {
			m_IsViewportInFocus = false;
			Mouse::SetVisible(true);
		}
	}

	if (e.Type == EventType::MouseButtonPress) {
		if (e.MouseButtonPress.Button == Mouse::Button::Left && m_Viewport.Inside({ e.MouseButtonPress.x, e.MouseButtonPress.y })) {
			m_IsViewportInFocus = true;
			Mouse::SetVisible(false);
		}
	}
}

