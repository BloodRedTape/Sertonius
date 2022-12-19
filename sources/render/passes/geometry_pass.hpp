#pragma once

#include <graphics/api/command_buffer.hpp>
#include <graphics/api/graphics_pipeline.hpp>
#include <graphics/api/descriptor_set.hpp>
#include "render/render_targets.hpp"
#include "render/mesh.hpp"
#include "render/scene.hpp"
#include "render/uniform_buffer.hpp"
#include "core/reusable_object_pool.hpp"

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
	
	UniquePtr<DescriptorSetLayout> m_SetLayout;
	UniquePtr<GraphicsPipeline> m_Pipeline{ nullptr };
	static constexpr size_t MaxUniformBuffers = 64;
	static constexpr size_t PreallocatedUniforms = 4;
	SingleFrameDescriptorSetPool m_SetPool{ {MaxUniformBuffers, m_SetLayout.Get()}, PreallocatedUniforms };
	ReusableObjectPool<UniformBuffer<ModelUniform>> m_ModelUniformBufferPool;

	UniformBuffer<CameraUniform> m_CameraUniform;
public:
	GeometryPass(const RenderTargets& targets);

	void CmdRender(CommandBuffer *cmd_buffer, const Scene &scene);
};