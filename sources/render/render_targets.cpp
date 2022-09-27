#include "render/render_targets.hpp"

RenderTargets::RenderTargets(const Window& window):
	FramebufferChain(&window, TextureFormat::Unknown),
	Albedo(
		Texture2D::Create(window.Size(), AlbedoFormat, TextureUsageBits::ColorAttachmentOptimal | TextureUsageBits::Sampled | TextureUsageBits::TransferDst, TextureLayout::ColorAttachmentOptimal)
	),
	Normal(
		Texture2D::Create(window.Size(), NormalFormat, TextureUsageBits::ColorAttachmentOptimal | TextureUsageBits::Sampled, TextureLayout::ColorAttachmentOptimal)
	),
	Position(
		Texture2D::Create(window.Size(), PositionFormat, TextureUsageBits::ColorAttachmentOptimal | TextureUsageBits::Sampled, TextureLayout::ColorAttachmentOptimal)
	),
	Material(
		Texture2D::Create(window.Size(), MaterialFormat, TextureUsageBits::ColorAttachmentOptimal | TextureUsageBits::Sampled, TextureLayout::ColorAttachmentOptimal)
	),
	Depth(
		Texture2D::Create(window.Size(), DepthFormat, TextureUsageBits::DepthStencilOptimal, TextureLayout::DepthStencilAttachmentOptimal)
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
			window.Size(),
			{
				Albedo.Get(),
				Normal.Get(),
				Position.Get(),
				Material.Get(),
				Depth.Get()
			},
			GeometryRenderPass.Get()
		})
	)
{}

void RenderTargets::Recreate(){
	const Window* window = PresentTarget();
	this->~RenderTargets();
	new (this) RenderTargets(*window);
}
