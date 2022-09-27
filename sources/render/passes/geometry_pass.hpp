#pragma once

#include <graphics/api/command_buffer.hpp>
#include <graphics/api/graphics_pipeline.hpp>
#include <graphics/api/descriptor_set.hpp>
#include "render/render_targets.hpp"
#include "render/mesh.hpp"
#include "render/scene.hpp"

class GeometryPass {
private:
	const RenderTargets& m_RenderTargets;
	
	UniquePtr<DescriptorSetLayout> m_Layout{
		DescriptorSetLayout::Create({})
	};
	UniquePtr<GraphicsPipeline> m_Pipeline{nullptr};
public:
	GeometryPass(const RenderTargets& targets);

	void CmdRender(CommandBuffer *cmd_buffer, const Scene &scene);
};