#pragma once

#include <graphics/api/command_buffer.hpp>
#include <graphics/api/compute_pipeline.hpp>
#include <graphics/api/descriptor_set.hpp>
#include "render/render_targets.hpp"
#include "render/mesh.hpp"

class CompositePass {
private:
	const RenderTargets& m_RenderTargets;
	
	UniquePtr<DescriptorSetLayout> m_Layout;
	UniquePtr<DescriptorSetPool> m_SetPool;
	UniquePtr<DescriptorSet, DescriptorSetDeleter> m_Set;
	UniquePtr<ComputePipeline> m_Pipeline;
	UniquePtr<Sampler> m_Sampler;
public:
	CompositePass(const RenderTargets& targets);

	void CmdRender(CommandBuffer *cmd_buffer);
};