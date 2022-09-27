#pragma once

#include <core/math/matrix4.hpp>

struct Camera {
	float FOV  = 0.f;
	float Near = 0.f;
	float Far  = 1.f;

	Matrix4f MakeProjectionMatrix()const;
};