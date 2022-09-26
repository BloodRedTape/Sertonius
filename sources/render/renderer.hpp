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
#include "components/mesh_component.hpp"

class Renderer {
private:
	const Window& m_Window;
	RenderTargets m_RenderTargets;

	GeometryPass m_GeometryPass{m_RenderTargets};
	CompositePass m_CompositePass{ m_RenderTargets };

	UniquePtr<CommandPool> m_Pool{ CommandPool::Create() };
	UniquePtr<CommandBuffer, CommandBufferDeleter> m_CmdBuffer{ m_Pool->Alloc(), {m_Pool.Get()} };
public:
	Renderer(const Window& window);

	void Render(Span<MeshComponent> meshes);

};