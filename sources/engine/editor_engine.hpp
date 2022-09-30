#pragma once

#include "engine/engine.hpp"
#include <graphics/render_texture.hpp>
#include <core/optional.hpp>

class EditorEngine : public Engine {
private:
	RenderTexture m_RenderTexture;
	Optional<Vector2s> m_PendingResize;
	Vector2s m_LastOutputFramebufferSize;
public:
	EditorEngine(Vector2s size, GameMode* game_mode);

	const Framebuffer* OutputFramebuffer()override;

	void OnImGui()override;

	void PostRender()override;
};