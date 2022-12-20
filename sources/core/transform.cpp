#include "transform.hpp"
#include <core/math/transform.hpp>

Matrix4f Transform::MakeTransformMatrix() const{
	Matrix4f scale{
		{Scale.x, 0.f, 0.f, 0.f},
		{0.f, Scale.y, 0.f, 0.f},
		{0.f, 0.f, Scale.z, 0.f},
		{0.f, 0.f, 0.f, 1.f}
	};
	return Math::Translate(Position) * Math::Rotate<float>(Math::Rad(Rotation)) * scale;
}