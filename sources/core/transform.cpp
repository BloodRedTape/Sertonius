#include "transform.hpp"
#include <core/math/transform.hpp>

Matrix4f Transform::MakeTransformMatrix() const{
	return Math::Translate(Position);
}