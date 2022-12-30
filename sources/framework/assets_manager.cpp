#include "assets_manager.hpp"
#include <assimp/BaseImporter.h>
#include <assimp/postprocess.h>
#include <assimp/version.h>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/DefaultLogger.hpp>
#include <assimp/vector3.h>

static List<UniquePtr<Texture2D>> m_Textures;
static List<Material> m_Materials;
static List<Mesh> m_Meshes;

TextureHandle AssetsManager::GetOrLoadTexture(StringView filepath) {
	return Add(UniquePtr<Texture2D>{ Texture2D::Create(filepath) });
}

static Vector3f ToVector3(aiVector3D vector){
    return {vector.x, vector.y, vector.z};
}

const aiNode* FindFirstMeshNode(const aiNode* root) {
    if (root->mNumMeshes)
        return root;

    for (int child_index = 0; child_index < root->mNumChildren; child_index++) {
        const aiNode* mesh = FindFirstMeshNode(root->mChildren[child_index]);

        if (mesh)
            return mesh;
    }

    return nullptr;
}

Mesh MakeMeshFromNode(const aiScene *scene, const aiNode *node, String name) {
    List<Vertex> vertices;
    List<Index> indices;
    List<MeshSection> sections;
    
    u32 vertex_offset = 0;
    u32 index_offset = 0;

    for (int i = 0; i < node->mNumMeshes; i++) {
        const aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        
        for (int j = 0; j < mesh->mNumVertices; j++) {
            vertices.Add({
                ToVector3(mesh->mVertices[j]),
                ToVector3(mesh->mNormals[j]),
            });
        }
        
        u32 indices_count = 0;
        for (int j = 0; j < mesh->mNumFaces; j++) {
            const aiFace& face = mesh->mFaces[j];

            for (int k = 0; k < face.mNumIndices; k++)
                indices.Add(face.mIndices[k]);

            indices_count += face.mNumIndices;
        }
        
        MeshSection section;
        section.BaseVertex = vertex_offset;
        section.BaseIndex = index_offset;
        section.IndicesCount = indices_count;
        sections.Add(section);

        vertex_offset += mesh->mNumVertices;
        index_offset += indices_count;
    }

    return Mesh(Move(sections), vertices, indices, AABB3f({ 0, 0 }, { 0, 0 }), name);
}

MeshHandle AssetsManager::GetOrLoadMesh(StringView filepath) {
        SX_ASSERT(File::Exists(filepath));

    Assimp::Importer imp;
    auto filedata = File::ReadEntire(filepath);
    assert(filedata.HasValue());
    String& data = filedata.Value();

    const aiScene *scene = imp.ReadFileFromMemory(data.Data(), data.Size(), aiProcess_Triangulate | aiProcess_GenNormals);

    const aiNode* node = FindFirstMeshNode(scene->mRootNode);

    assert(node);

    return Add(MakeMeshFromNode(scene, node, filepath));
}

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
