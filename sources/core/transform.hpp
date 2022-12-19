#pragma once


#include <core/math/vector3.hpp>
#include <core/math/matrix4.hpp>

struct Transform {
	Vector3f Position;
	Vector3f Rotation;
	Vector3f Scale{ 1.f, 1.f, 1.f };

	Matrix4f MakeTransformMatrix()const;
};