#include "render/render_targets.hpp"

RenderTargets::RenderTargets(Vector2s size):
	Albedo(
		Texture2D::Create(size, AlbedoFormat, TextureUsageBits::ColorAttachmentOptimal | TextureUsageBits::Sampled | TextureUsageBits::TransferDst, TextureLayout::ColorAttachmentOptimal)
	),
	Normal(
		Texture2D::Create(size, NormalFormat, TextureUsageBits::ColorAttachmentOptimal | TextureUsageBits::Sampled | TextureUsageBits::TransferDst, TextureLayout::ColorAttachmentOptimal)
	),
	Position(
		Texture2D::Create(size, PositionFormat, TextureUsageBits::ColorAttachmentOptimal | TextureUsageBits::Sampled | TextureUsageBits::TransferDst, TextureLayout::ColorAttachmentOptimal)
	),
	Material(
		Texture2D::Create(size, MaterialFormat, TextureUsageBits::ColorAttachmentOptimal | TextureUsageBits::Sampled | TextureUsageBits::TransferDst, TextureLayout::ColorAttachmentOptimal)
	),
	Depth(
		Texture2D::Create(size, DepthFormat, TextureUsageBits::DepthStencilOptimal | TextureUsageBits::Sampled | TextureUsageBits::TransferDst, TextureLayout::DepthStencilAttachmentOptimal)
	),
	Lighting(
		Texture2D::Create(size, LightingFormat, TextureUsageBits::ColorAttachmentOptimal | TextureUsageBits::Sampled | TextureUsageBits::TransferDst, TextureLayout::ColorAttachmentOptimal)
	),
	GeometryRenderPass(
		RenderPass::Create(RenderPassProperties{
			ConstSpan<AttachmentDescription>( {
				AttachmentDescription{
					TextureLayout::ColorAttachmentOptimal,
					TextureLayout::ColorAttachmentOptimal,
					TextureLayout::ColorAttachmentOptimal,
					Albedo->Format(),
					SamplePoints::Samples_1
				},
				AttachmentDescription{
					TextureLayout::ColorAttachmentOptimal,
					TextureLayout::ColorAttachmentOptimal,
					TextureLayout::ColorAttachmentOptimal,
					Normal->Format(),
					SamplePoints::Samples_1
				},
				AttachmentDescription{
					TextureLayout::ColorAttachmentOptimal,
					TextureLayout::ColorAttachmentOptimal,
					TextureLayout::ColorAttachmentOptimal,
					Position->Format(),
					SamplePoints::Samples_1
				},
				AttachmentDescription{
					TextureLayout::ColorAttachmentOptimal,
					TextureLayout::ColorAttachmentOptimal,
					TextureLayout::ColorAttachmentOptimal,
					Material->Format(),
					SamplePoints::Samples_1
				},
				AttachmentDescription{
					TextureLayout::DepthStencilAttachmentOptimal,
					TextureLayout::DepthStencilAttachmentOptimal,
					TextureLayout::DepthStencilAttachmentOptimal,
					Depth->Format(),
					SamplePoints::Samples_1
				}
			})
		})
	),
	GeometryFrameBuffer(
		Framebuffer::Create({
			size,
			{
				Albedo.Get(),
				Normal.Get(),
				Position.Get(),
				Material.Get(),
				Depth.Get()
			},
			GeometryRenderPass.Get()
		})
	),
	LightingRenderPass(
		RenderPass::Create(RenderPassProperties{
			ConstSpan<AttachmentDescription>({
				AttachmentDescription{
					TextureLayout::ColorAttachmentOptimal,
					TextureLayout::ColorAttachmentOptimal,
					TextureLayout::ColorAttachmentOptimal,
					Lighting->Format(),
					SamplePoints::Samples_1
				}
			})
		})
	),
	LightingFrameBuffer(
		Framebuffer::Create({
			size,
			{
				Lighting.Get()
			},
			LightingRenderPass.Get()
		})
	)
{}

void RenderTargets::OnFramebufferRecreate(Vector2s size){
	auto light = Move(LightingRenderPass);
	auto geom = Move(GeometryRenderPass);
	this->~RenderTargets();
	new (this) RenderTargets(size);
	LightingRenderPass = Move(light);
	GeometryRenderPass = Move(geom);
}
