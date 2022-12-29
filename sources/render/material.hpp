#pragma once

#include <core/math/vector3.hpp>

using TextureId = u32;

constexpr TextureId InvalidTextureId = -1;

struct Material {
	Vector3f ColorValue;
	TextureId ColorTextureId = InvalidTextureId;
};