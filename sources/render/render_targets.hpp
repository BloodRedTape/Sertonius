#pragma once

#include <graphics/api/framebuffer.hpp>
#include <graphics/api/swapchain.hpp>

struct RenderTargets{
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

	UniquePtr<RenderPass>  GeometryRenderPass;
	UniquePtr<Framebuffer> GeometryFrameBuffer;
	
	RenderTargets(Vector2s size);

	void OnRecreate(FramebufferChain *);

	Vector2s Size()const {
		return GeometryFrameBuffer->Size();
	}
};

