#pragma once

#include "engine/engine.hpp"
#include <graphics/render_texture.hpp>
#include <core/optional.hpp>
#include <core/math/aabb2.hpp>

class EditorEngine : public Engine {
private:
	RenderTexture m_RenderTexture;
	Optional<Vector2s> m_PendingResize;
	Vector2s m_LastOutputFramebufferSize;
	AABB2s m_Viewport{ {}, {} };
	Vector2s m_LastMousePosition;
public:
	EditorEngine(Vector2s size, GameMode* game_mode);

	const Framebuffer* OutputFramebuffer()override;

	void OnImGui()override;

	void Tick(float dt)override;

	void PostRender()override;

	void OnEvent(const Event& e)override;
};