#include "components/mesh_component.hpp"

MeshComponent::MeshComponent(Mesh mesh):
	Mesh(Move(mesh))
{}
