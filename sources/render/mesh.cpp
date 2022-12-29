#include "render/mesh.hpp"
#include <assimp/BaseImporter.h>
#include <assimp/postprocess.h>
#include <assimp/version.h>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/DefaultLogger.hpp>
#include <assimp/vector3.h>
#include <core/list.hpp>
#include <string>
#include <core/os/file.hpp>
#include <cassert>

Array<VertexAttribute, 2> Vertex::AttributesList = {
    VertexAttribute::Float32x3,
    VertexAttribute::Float32x3
};

Mesh::Mesh(ConstSpan<Vertex> vertices, ConstSpan<Index> indices, const AABB3f &bounding_box, String name):
    Mesh({ MeshSection{0, 0, u32(indices.Size())} }, vertices, indices, bounding_box, Move(name))
{}

Mesh::Mesh(List<MeshSection> sections, ConstSpan<Vertex> vertices, ConstSpan<Index> indices, const AABB3f & bounding_box, String name):
	m_VertexBuffer(
		Buffer::Create(
			vertices.Pointer(), vertices.Size() * sizeof(Vertex), 
			BufferMemoryType::DynamicVRAM, 
			BufferUsageBits::VertexBuffer | BufferUsageBits::TransferDestination
		)
	),
	m_IndexBuffer(
		Buffer::Create(
			indices.Pointer(), indices.Size() * sizeof(Index), 
			BufferMemoryType::DynamicVRAM, 
			BufferUsageBits::IndexBuffer| BufferUsageBits::TransferDestination
		)
	),
    m_Sections(Move(sections)),
    m_BoundingBox(bounding_box),
    m_Name(Move(name))
{}

void Mesh::CmdDraw(CommandBuffer& buffer)const{
	buffer.BindVertexBuffer(m_VertexBuffer.Get());
	buffer.BindIndexBuffer(m_IndexBuffer.Get(), IndicesType::Uint32);
    for(const MeshSection &section: m_Sections)
        buffer.DrawIndexed(section.IndicesCount, section.BaseIndex, section.BaseVertex);
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

Mesh Mesh::LoadFromFile(StringView filepath) {

    SX_ASSERT(File::Exists(filepath));

    Assimp::Importer imp;
    auto filedata = File::ReadEntire(filepath);
    assert(filedata.HasValue());
    String& data = filedata.Value();

    const aiScene *scene = imp.ReadFileFromMemory(data.Data(), data.Size(), aiProcess_Triangulate | aiProcess_GenNormals);

    const aiNode* node = FindFirstMeshNode(scene->mRootNode);

    assert(node);

    return MakeMeshFromNode(scene, node, filepath);
}
