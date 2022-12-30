#include "actor_loader.hpp"
#include <assimp/BaseImporter.h>
#include <assimp/postprocess.h>
#include <assimp/version.h>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/DefaultLogger.hpp>
#include <assimp/vector3.h>

static inline Vector3f ToVector3(aiVector3D vector){
    return {vector.x, vector.y, vector.z};
}

Mesh ActorLoader::MakeMeshFromNode(const aiNode *node, String name) {

    List<Vertex> vertices;
    List<Index> indices;
    List<MeshSection> sections;
    
    u32 vertex_offset = 0;
    u32 index_offset = 0;

    for (int i = 0; i < node->mNumMeshes; i++) {
        const aiMesh* mesh = m_Scene->mMeshes[node->mMeshes[i]];
        
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

WeakActorPtr<Actor> ActorLoader::MakeActorFromNode(const class aiNode* node) {
    auto actor_ptr = m_World->Spawn(Actor());
    
    if (node->mNumMeshes) {
        MeshHandle mesh = AssetsManager::Add(MakeMeshFromNode(node, {}));
        actor_ptr.Pin()->AddComponent<MeshComponent>(mesh);
    }
    aiVector3D scaling;
    aiVector3D rotation;
    aiVector3D position;
    node->mTransformation.Decompose(scaling, rotation, position);
    actor_ptr.Pin()->Position = ToVector3(position);
    actor_ptr.Pin()->Rotation = Math::Deg(ToVector3(rotation));
    actor_ptr.Pin()->Scale = ToVector3(scaling);

    for (int i = 0; i < node->mNumChildren; i++)
        actor_ptr.Pin()->AttachChild(MakeActorFromNode(node->mChildren[i]).Pin());

    return actor_ptr;
}

MaterialHandle ActorLoader::GetMaterial(int material_index) {
    auto it = m_MaterialsMap.find(material_index);

    if (it == m_MaterialsMap.end()) {
        Material mat;
        MaterialHandle handle = AssetsManager::Add(mat);
        it = m_MaterialsMap.insert({ material_index, handle }).first;
    }

    return it->second;
}

ActorLoader::ActorLoader(World* world, StringView filepath):
	m_World(world)
{
	SX_ASSERT(File::Exists(filepath));
    
    m_Importer = new Assimp::Importer();
    auto filedata = File::ReadEntire(filepath);
    assert(filedata.HasValue());
    String& data = filedata.Value();

    m_Scene = m_Importer->ReadFileFromMemory(data.Data(), data.Size(), aiProcess_Triangulate | aiProcess_GenNormals);
}

ActorLoader::~ActorLoader() {
    delete m_Importer;
}

WeakActorPtr<Actor> ActorLoader::Load() {
    return MakeActorFromNode(m_Scene->mRootNode);
}