#include "actor_loader.hpp"
#include <assimp/BaseImporter.h>
#include <assimp/postprocess.h>
#include <assimp/version.h>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/DefaultLogger.hpp>
#include <assimp/vector3.h>
#include <core/print.hpp>
#include <core/os/clock.hpp>

class ScopeTimer {
    Clock m_Clock;
    const char* m_ScopeName;
public:
    ScopeTimer(const char *name):
        m_ScopeName(name)
    {}

    ~ScopeTimer() {
        Println("Scope: % took %ms", m_ScopeName, m_Clock.GetElapsedTime().AsMilliseconds());
    }
};

#define CONCAT_IMPL(a, b) a##b
#define CONCAT(a, b) CONCAT_IMPL(a, b)

#if 0
#define PROFILE_SCOPE(name) ScopeTimer CONCAT(__, SX_THIS_LINE) (name)
#define PROFILE_FUNC() PROFILE_SCOPE(SX_THIS_FUNCTION)
#else
#define PROFILE_SCOPE(name)
#define PROFILE_FUNC()
#endif

static inline Vector3f ToVector3(aiVector3D vector){
    return {vector.x, vector.y, vector.z};
}
static inline Vector2f ToVector2(aiVector3D vector){
    return {vector.x, vector.y};
}

Mesh ActorLoader::MakeMeshFromNode(const aiNode *node, String name) {
    PROFILE_FUNC();
    List<Vertex> vertices;
    List<Index> indices;
    List<MeshSection> sections;
    
    u32 vertex_offset = 0;
    u32 index_offset = 0;

    for (int i = 0; i < node->mNumMeshes; i++) {
        const aiMesh* mesh = m_Scene->mMeshes[node->mMeshes[i]];
        SX_ASSERT(mesh->mNumUVComponents[0] == 2);
        SX_ASSERT(!mesh->mNumUVComponents[1]);

        for (int j = 0; j < mesh->mNumVertices; j++) {
            vertices.Add({
                ToVector3(mesh->mVertices[j]),
                ToVector3(mesh->mNormals[j]),
                ToVector3(mesh->mTangents[j]),
                ToVector2(mesh->mTextureCoords[0][j]),
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
        section.MaterialIndex = GetMaterial(mesh->mMaterialIndex);
        sections.Add(section);

        vertex_offset += mesh->mNumVertices;
        index_offset += indices_count;
    }

    for (Vertex& vertex : vertices)
        vertex.Position *= 0.01f;

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

void PopulateAlbedo(Material &mat, class aiMaterial *in_mat) {
    mat.ColorValue = { 1, 1, 1 };

}

class MaterialLoader {
private:
    aiMaterial* m_Source = nullptr;
    String m_ActorFileDirectory;
public:
    MaterialLoader(aiMaterial* material, String actor_file_directory):
        m_Source(material),
        m_ActorFileDirectory(Move(actor_file_directory))
    {}

    void LoadAlbedo(Material& out) {
        PROFILE_FUNC();
        {
            aiColor3D color_value;
            if (m_Source->Get(AI_MATKEY_COLOR_DIFFUSE, color_value) == aiReturn_SUCCESS)
                out.ColorValue = { color_value.r, color_value.g, color_value.b };
        }
        {
            if (m_Source->GetTextureCount(aiTextureType_DIFFUSE)) {
                aiString local_texture_path;
                if (m_Source->GetTexture(aiTextureType_DIFFUSE, 0, &local_texture_path) == aiReturn_SUCCESS) {
                    String full_texture_path = m_ActorFileDirectory + local_texture_path.C_Str();
                    if (File::Exists(full_texture_path))
                        out.ColorTextureIndex = AssetsManager::GetOrLoadTexture(full_texture_path);
                }
            }
        }
    }

    void LoadNormal(Material& out) {
        PROFILE_FUNC();
        if (m_Source->GetTextureCount(aiTextureType_HEIGHT)) {
            aiString local_texture_path;
            if (m_Source->GetTexture(aiTextureType_HEIGHT, 0, &local_texture_path) == aiReturn_SUCCESS) {
                String full_texture_path = m_ActorFileDirectory + local_texture_path.C_Str();
                Println("LoadingNormalMap: %", full_texture_path);
                if (File::Exists(full_texture_path))
                    out.NormalTextureIndex = AssetsManager::GetOrLoadTexture(full_texture_path);
            }

        }
    }
};

MaterialHandle ActorLoader::GetMaterial(int material_index) {
    PROFILE_FUNC();
    auto it = m_MaterialsMap.find(material_index);

    if (it == m_MaterialsMap.end()) {
        Material mat;

        MaterialLoader loader(m_Scene->mMaterials[material_index], m_FileDirectory);
        loader.LoadAlbedo(mat);
        loader.LoadNormal(mat);
        MaterialHandle handle = AssetsManager::Add(mat);

        it = m_MaterialsMap.insert({ material_index, handle }).first;
    }

    return it->second;
}

ActorLoader::ActorLoader(World* world, StringView filepath):
	m_World(world)
{
    PROFILE_FUNC();
	SX_ASSERT(File::Exists(filepath));

    m_FileDirectory = filepath;
    char *last_delimiter = String::FindLast(m_FileDirectory.Data(), "/") + 1;
    m_FileDirectory = String{ m_FileDirectory.Data(), size_t(last_delimiter - m_FileDirectory.Data())};
    Println("FileDir: %", m_FileDirectory);
    
    m_Importer = new Assimp::Importer();
    auto filedata = File::ReadEntire(filepath);
    SX_ASSERT(filedata.HasValue());
    String& data = filedata.Value();
    u32 flags = aiProcess_Triangulate | aiProcess_ConvertToLeftHanded | aiProcess_GenNormals | aiProcess_CalcTangentSpace;
    m_Scene = m_Importer->ReadFileFromMemory(data.Data(), data.Size(), flags);
}

ActorLoader::~ActorLoader() {
    delete m_Importer;
}

WeakActorPtr<Actor> ActorLoader::Load() {
    PROFILE_FUNC();
    auto actor = MakeActorFromNode(m_Scene->mRootNode);
    actor.Pin()->Rotation.x += 90;
    return actor;
}