#pragma once

#include <core/math/vector3.hpp>

constexpr u32 InvalidTextureIndex = -1;

struct Material {
	Vector3f ColorValue{ 1.f, 1.f, 1.f };
	u32 ColorTextureIndex = InvalidTextureIndex;
};