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
	List<UniquePtr<Texture2D>> m_Textures;
	List<Material> m_Materials;
	List<Mesh> m_Meshes;
private:
	AssetsManager() = default;
public:
	TextureHandle GetOrLoadTexture(StringView filepath);

	MeshHandle GetOrLoadMesh(StringView filepath);

	MeshHandle Add(Mesh mesh);

	MaterialHandle Add(Material mat);

	TextureHandle Add(UniquePtr<Texture2D> tex);

	const Mesh *Get(MeshHandle handle)const;

	const Material *Get(MaterialHandle handle)const;

	const Texture2D *Get(TextureHandle handle)const;

	static AssetsManager& Get();
};