#include "render/renderer.hpp"
#include <graphics/api/gpu.hpp>
#include <core/unique_ptr.hpp>

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

Renderer::Renderer(const Window &window):
	m_Window(window),
	m_RenderTargets(window)
{}

Semaphore acq, pst;
Fence fence;

//int s = (fence.Signal(), 1);

void Renderer::Render(){
	m_RenderTargets.AcquireNext(&acq);

	m_CmdBuffer->Begin();
	m_CmdBuffer->ClearColor(m_RenderTargets.CurrentFramebuffer()->Attachments()[0], Color::Red);
	m_CmdBuffer->End();

	GPU::Execute(m_CmdBuffer.Get(), acq, pst, fence);
	fence.WaitAndReset();
	m_RenderTargets.PresentCurrent(&pst);
}
