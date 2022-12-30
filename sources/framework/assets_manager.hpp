#pragma once

#include <graphics/api/texture.hpp>
#include "render/mesh.hpp"
#include "render/material.hpp"
#include "core/gen.hpp"

GENERATE_HANDLE_CLASS(MeshHandle, u32, -1)
GENERATE_HANDLE_CLASS(TextureHandle, u32, -1)
GENERATE_HANDLE_CLASS(MaterialHandle, u32, -1)

class AssetsManager {
private:
private:
	AssetsManager() = default;
public:
	static TextureHandle GetOrLoadTexture(StringView filepath);

	static MeshHandle Add(Mesh mesh);

	static MaterialHandle Add(Material mat);

	static TextureHandle Add(UniquePtr<Texture2D> tex);

	static const Mesh *Get(MeshHandle handle);

	static const Material *Get(MaterialHandle handle);

	static const Texture2D *Get(TextureHandle handle);

	static ConstSpan<UniquePtr<Texture2D>> Textures();
};