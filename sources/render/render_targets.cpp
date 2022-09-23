#include "render/render_targets.hpp"

RenderTargets::RenderTargets(const Window& window):
	FramebufferChain(&window, TextureFormat::Unknown),
	Albedo(
		Texture2D::Create(window.Size(), AlbedoFormat, TextureUsageBits::ColorAttachmentOptimal | TextureUsageBits::Sampled, TextureLayout::ColorAttachmentOptimal)
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
	)
{}

void RenderTargets::Recreate(){
	const Window* window = PresentTarget();
	this->~RenderTargets();
	new (this) RenderTargets(*window);
}
