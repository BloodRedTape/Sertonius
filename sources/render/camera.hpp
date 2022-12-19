#pragma once

#include <core/math/matrix4.hpp>

struct Camera {
	float FOV    = 90.f;
	float Near   = 0.f;
	float Far    = 1.f;
	float Aspect = 16.f / 9.f;

	Matrix4f MakeProjectionMatrix()const;
};