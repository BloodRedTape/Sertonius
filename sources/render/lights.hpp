#pragma once

#include <core/math/vector2.hpp>
#include <core/math/vector3.hpp>

struct PointLight {
	Vector3f Color = { 1.f, 1.f, 1.f };
	float Radius = 10.f;
};