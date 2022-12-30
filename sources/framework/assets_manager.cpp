#include "assets_manager.hpp"

static List<UniquePtr<Texture2D>> m_Textures;
static List<Material> m_Materials;
static List<Mesh> m_Meshes;

MeshHandle AssetsManager::Add(Mesh mesh) {
	MeshHandle handle = m_Meshes.Size();
	m_Meshes.Add(Move(mesh));
	return handle;
}

MaterialHandle AssetsManager::Add(Material mat) {
	MaterialHandle handle = m_Materials.Size();
	m_Materials.Add(Move(mat));
	return handle;
}

TextureHandle AssetsManager::Add(UniquePtr<Texture2D> tex) {
	TextureHandle handle = m_Textures.Size();
	m_Textures.Add(Move(tex));
	return handle;
}

const Mesh* AssetsManager::Get(MeshHandle handle) {
	if (!handle)
		return nullptr;
	return &m_Meshes[(u32)handle];
}

const Material* AssetsManager::Get(MaterialHandle handle){
	if (!handle)
		return nullptr;
	return &m_Materials[(u32)handle];
}

const Texture2D* AssetsManager::Get(TextureHandle handle){
	if (!handle)
		return nullptr;

	return m_Textures[(u32)handle].Get();
}
