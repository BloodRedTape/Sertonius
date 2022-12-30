#include "render/mesh.hpp"

#include <core/list.hpp>
#include <string>
#include <core/os/file.hpp>
#include <cassert>

Array<VertexAttribute, 3> Vertex::AttributesList = {
    VertexAttribute::Float32x3,
    VertexAttribute::Float32x3,
    VertexAttribute::Float32x2,
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

void Mesh::Bind(CommandBuffer& buffer)const{
	buffer.BindVertexBuffer(m_VertexBuffer.Get());
	buffer.BindIndexBuffer(m_IndexBuffer.Get(), IndicesType::Uint32);
}


