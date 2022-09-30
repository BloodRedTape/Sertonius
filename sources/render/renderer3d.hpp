#pragma once

#include <core/math/vector2.hpp>
#include <core/unique_ptr.hpp>
#include <core/os/window.hpp>
#include <graphics/api/graphics_pipeline.hpp>
#include <graphics/api/semaphore.hpp>
#include <graphics/api/command_buffer.hpp>
#include "render/mesh.hpp"
#include "render/render_targets.hpp"
#include "render/passes/geometry_pass.hpp"
#include "render/passes/composite_pass.hpp"
#include "render/scene.hpp"
#include "imgui/backend.hpp"

class Renderer3D{
private:
	const RenderPass* m_Pass;
	RenderTargets m_RenderTargets;

	GeometryPass m_GeometryPass{m_RenderTargets};
	CompositePass m_CompositePass{ m_RenderTargets };
public:
	Renderer3D(const FramebufferChain &chain);

	void CmdRender(CommandBuffer *cmd_buffer, const Framebuffer *fb, const Scene &scene);
	
	void OnImGui();

	void OnFramebufferRecreate(Vector2s size);
};