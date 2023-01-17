#pragma once

#include <graphics/api/command_buffer.hpp>
#include <graphics/api/graphics_pipeline.hpp>
#include <graphics/api/descriptor_set.hpp>
#include "render/render_targets.hpp"
#include "render/scene.hpp"
#include "render/uniform_buffer.hpp"

struct LightVertex {
	Vector2f VertexPosition;
	Vector3f LightPosition;
	Vector3f LightColor;
	float LightRadius;

	static Array<VertexAttribute, 4> AttributesList;
};

class LightingPass {
	struct CameraUniform {
		Vector3f u_CameraLocation;
	};

	const RenderTargets& m_RenderTargets;

	UniquePtr<Sampler> m_Sampler{ Sampler::Create({}) };

	UniquePtr<DescriptorSetLayout> m_SetLayout;
	UniquePtr<GraphicsPipeline> m_Pipeline{ nullptr };
	
	UniquePtr<DescriptorSetPool> m_SetPool{ DescriptorSetPool::Create({1, m_SetLayout.Get()}) };
	UniquePtr<DescriptorSet, DescriptorSetDeleter> m_Set{ m_SetPool->Alloc(), {m_SetPool.Get()} };

	UniquePtr<Buffer> m_LightGeometry{Buffer::Create(sizeof(LightVertex)*6, BufferMemoryType::DynamicVRAM, BufferUsageBits::VertexBuffer | BufferUsageBits::TransferDestination)};

	UniformBuffer<CameraUniform> m_CameraUniformBuffer;
public:
	LightingPass(const RenderTargets& rts);

	void CmdRender(CommandBuffer* cmd_buffer, const Scene &scene);
};