#pragma once

#include <core/math/vector2.hpp>
#include <core/unique_ptr.hpp>
#include <core/os/window.hpp>
#include <graphics/api/graphics_pipeline.hpp>
#include <graphics/api/semaphore.hpp>
#include <graphics/api/framebuffer.hpp>
#include <graphics/api/swapchain.hpp>
#include <graphics/api/command_buffer.hpp>

struct RenderTargets: FramebufferChain{
	static constexpr TextureFormat AlbedoFormat = TextureFormat::RGBA8;
	static constexpr TextureFormat NormalFormat = TextureFormat::RGBA16F;
	static constexpr TextureFormat PositionFormat = TextureFormat::RGBA16F;
	static constexpr TextureFormat MaterialFormat = TextureFormat::RGBA8;
	static constexpr TextureFormat DepthFormat = TextureFormat::Depth32;

	UniquePtr<Texture2D> Albedo;
	UniquePtr<Texture2D> Normal;
	UniquePtr<Texture2D> Position;
	UniquePtr<Texture2D> Material;
	UniquePtr<Texture2D> Depth;
	
	RenderTargets(const Window &window);

	void Recreate()override;
};

class Renderer {
private:
	const Window& m_Window;
	RenderTargets m_RenderTargets;
	UniquePtr<CommandPool> m_Pool{ CommandPool::Create() };
	UniquePtr<CommandBuffer, CommandBufferDeleter> m_CmdBuffer{ m_Pool->Alloc(), {m_Pool.Get()} };
public:
	Renderer(const Window& window);

	void Render();

};