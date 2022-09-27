#pragma once

#include <core/math/matrix4.hpp>

struct Camera {
	float FOV  = 0.f;
	float Near = 0.f;
	float Far  = 1.f;

	Matrix4f MakeProjectionMatrix()const;

	Matrix4f MakeViewMatrix(const Vector3f &position, const Vector3f &rotation)const;
};