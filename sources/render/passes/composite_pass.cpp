#include "render/passes/composite_pass.hpp"
#include <cmath>

CompositePass::CompositePass(const RenderTargets& targets) :
	m_RenderTargets(targets),
	m_Layout(
		DescriptorSetLayout::Create({
			ShaderBinding(0, 1, ShaderBindingType::Texture, ShaderStageBits::Compute),
			ShaderBinding(1, 1, ShaderBindingType::Texture, ShaderStageBits::Compute),
			ShaderBinding(2, 1, ShaderBindingType::Texture, ShaderStageBits::Compute),
			ShaderBinding(3, 1, ShaderBindingType::Texture, ShaderStageBits::Compute),
			ShaderBinding(4, 1, ShaderBindingType::Texture, ShaderStageBits::Compute),
			ShaderBinding(5, 1, ShaderBindingType::StorageTexture, ShaderStageBits::Compute)
			})
	),
	m_SetPool(
		DescriptorSetPool::Create({
			1,
			m_Layout.Get()
		})
	),
	m_Set(m_SetPool->Alloc(), {m_SetPool.Get()}),
	m_Pipeline(nullptr),
	m_Sampler(
		Sampler::Create({})
	)
{
	const Shader* shader = Shader::Create(ShaderLang::GLSL, ShaderStageBits::Compute, File::ReadEntire("shaders/comp.comp.glsl").Value());
	
	m_Pipeline = ComputePipeline::Create({
		shader,
		m_Layout.Get()
	});

	delete shader;
}

void CompositePass::CmdRender(CommandBuffer* cmd_buffer, const Framebuffer *fb){
	const Texture2D* PresentTexture = fb->Attachments()[0];

	m_Set->UpdateTextureBinding(0, 0, m_RenderTargets.Albedo.Get(), m_Sampler.Get());
	m_Set->UpdateTextureBinding(1, 0, m_RenderTargets.Normal.Get(), m_Sampler.Get());
	m_Set->UpdateTextureBinding(2, 0, m_RenderTargets.Position.Get(), m_Sampler.Get());
	m_Set->UpdateTextureBinding(3, 0, m_RenderTargets.Material.Get(), m_Sampler.Get());
	m_Set->UpdateTextureBinding(4, 0, m_RenderTargets.Lighting.Get(), m_Sampler.Get());
	m_Set->UpdateStorageTextureBinding(5, 0, PresentTexture);

	cmd_buffer->Bind(m_Pipeline.Get());
	cmd_buffer->Bind(m_Set.Get());
	cmd_buffer->ChangeLayout(m_RenderTargets.Albedo.Get(), TextureLayout::ShaderReadOnlyOptimal);
	cmd_buffer->ChangeLayout(m_RenderTargets.Normal.Get(), TextureLayout::ShaderReadOnlyOptimal);
	cmd_buffer->ChangeLayout(m_RenderTargets.Position.Get(), TextureLayout::ShaderReadOnlyOptimal);
	cmd_buffer->ChangeLayout(m_RenderTargets.Material.Get(), TextureLayout::ShaderReadOnlyOptimal);
	cmd_buffer->ChangeLayout(m_RenderTargets.Lighting.Get(), TextureLayout::ShaderReadOnlyOptimal);
	cmd_buffer->ChangeLayout(PresentTexture, TextureLayout::General);
	cmd_buffer->Dispatch(std::ceil(fb->Size().x/16.f), std::ceil(fb->Size().y/16.f), 1);
	cmd_buffer->ChangeLayout(m_RenderTargets.Albedo.Get(), TextureLayout::ColorAttachmentOptimal);
	cmd_buffer->ChangeLayout(m_RenderTargets.Normal.Get(), TextureLayout::ColorAttachmentOptimal);
	cmd_buffer->ChangeLayout(m_RenderTargets.Position.Get(), TextureLayout::ColorAttachmentOptimal);
	cmd_buffer->ChangeLayout(m_RenderTargets.Material.Get(), TextureLayout::ColorAttachmentOptimal);
	cmd_buffer->ChangeLayout(m_RenderTargets.Lighting.Get(), TextureLayout::ColorAttachmentOptimal);
	cmd_buffer->ChangeLayout(PresentTexture, TextureLayout::PresentSrcOptimal);
}

