#pragma once

#include <graphics/api/framebuffer.hpp>
#include <graphics/api/swapchain.hpp>

struct RenderTargets{
	static constexpr TextureFormat AlbedoFormat = TextureFormat::RGBA8;
	static constexpr TextureFormat NormalFormat = TextureFormat::RGBA16F;
	static constexpr TextureFormat PositionFormat = TextureFormat::RGBA16F;
	static constexpr TextureFormat MaterialFormat = TextureFormat::RGBA8;
	static constexpr TextureFormat DepthFormat = TextureFormat::Depth32;
	static constexpr TextureFormat LightingFormat = TextureFormat::RGBA8;

	UniquePtr<Texture2D> Albedo;
	UniquePtr<Texture2D> Normal;
	UniquePtr<Texture2D> Position;
	UniquePtr<Texture2D> Material;
	UniquePtr<Texture2D> Depth;
	UniquePtr<Texture2D> Lighting;

	UniquePtr<RenderPass>  GeometryRenderPass;
	UniquePtr<Framebuffer> GeometryFrameBuffer;

	UniquePtr<RenderPass>  LightingRenderPass;
	UniquePtr<Framebuffer> LightingFrameBuffer;
	
	RenderTargets(Vector2s size);

	void OnFramebufferRecreate(Vector2s size);

	Vector2s Size()const {
		return GeometryFrameBuffer->Size();
	}
};

