#pragma once

#include <core/math/vector3.hpp>

constexpr u32 InvalidTextureIndex = -1;

struct Material {
	Vector3f ColorValue;
	u32 ColorTextureIndex = InvalidTextureIndex;
};