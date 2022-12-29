#pragma once

#include <core/span.hpp>
#include <core/math/vector3.hpp>
#include <core/math/aabb3.hpp>
#include <core/unique_ptr.hpp>
#include <core/string.hpp>
#include <core/array.hpp>
#include <core/list.hpp>
#include <graphics/api/buffer.hpp>
#include <graphics/api/command_buffer.hpp>
#include <graphics/api/graphics_pipeline.hpp>

struct Vertex {
	Vector3f Position;
	Vector3f Normal;

	static Array<VertexAttribute, 2> AttributesList;
};

struct MeshSection {
	u32 BaseVertex = 0;
	u32 BaseIndex = 0;
	u32 IndicesCount = 0;
};

using Index = u32;

class Mesh {
private:
	UniquePtr<Buffer> m_VertexBuffer;
	UniquePtr<Buffer> m_IndexBuffer;
	List<MeshSection> m_Sections;
	String m_Name;
	AABB3f m_BoundingBox;
public:
	Mesh(ConstSpan<Vertex> vertices, ConstSpan<Index> indices, const AABB3f &bounding_box, String name = "");

	Mesh(List<MeshSection> sections, ConstSpan<Vertex> vertices, ConstSpan<Index> indices, const AABB3f &bounding_box, String name = "");

	Mesh(Mesh &&) = default;

	Mesh &operator=(Mesh &&) = default;

	void CmdDraw(CommandBuffer &buffer)const;

	const String& Name()const {
		return m_Name;
	}

	const AABB3f &BoundingBox()const {
		return m_BoundingBox;
	}

	static Mesh LoadFromFile(StringView filepath);
};