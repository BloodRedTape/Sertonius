#pragma once

#include <unordered_map>
#include "actor.hpp"
#include "assets_manager.hpp"

namespace Assimp {
	class Importer;
}

class ActorLoader {
private:
	std::unordered_map<int, MaterialHandle> m_MaterialsMap;
	const class aiScene* m_Scene = nullptr;
	Assimp::Importer* m_Importer = nullptr;
	World* m_World = nullptr;
	String m_FileDirectory;
public:
	ActorLoader(World* world, StringView filepath);

	~ActorLoader();

	WeakActorPtr<Actor> Load();

private:
	Mesh MakeMeshFromNode(const class aiNode* node, String name);

	WeakActorPtr<Actor> MakeActorFromNode(const class aiNode* node);

	MaterialHandle GetMaterial(int material_index);
};