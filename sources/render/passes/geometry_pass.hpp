#pragma once

#include <graphics/api/command_buffer.hpp>
#include <graphics/api/graphics_pipeline.hpp>
#include <graphics/api/descriptor_set.hpp>
#include "render/render_targets.hpp"
#include "render/mesh.hpp"
#include "render/scene.hpp"
#include "render/uniform_buffer.hpp"

class GeometryPass {
	struct CameraUniform {
		Matrix4f u_View;
		Matrix4f u_Projection;
	};

	struct ModelUniform {
		Matrix4f u_Model;
	};
private:
	const RenderTargets& m_RenderTargets;
	
	UniquePtr<DescriptorSetLayout> m_Layout;
	UniquePtr<GraphicsPipeline> m_Pipeline{ nullptr };
	UniquePtr<DescriptorSetPool> m_SetPool{ DescriptorSetPool::Create({1, m_Layout.Get()}) };
	UniquePtr<DescriptorSet, DescriptorSetDeleter> m_Set{ m_SetPool->Alloc(), {m_SetPool.Get()} };

	UniformBuffer<CameraUniform> m_CameraUniform;
public:
	GeometryPass(const RenderTargets& targets);

	void CmdRender(CommandBuffer *cmd_buffer, const Scene &scene);
};