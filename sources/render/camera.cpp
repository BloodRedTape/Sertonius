#include "render/camera.hpp"

Matrix4f Camera::MakeProjectionMatrix()const{
	return Matrix4f{ 1.f };
}

Matrix4f Camera::MakeViewMatrix(const Vector3f& position, const Vector3f& rotation) const{
	Matrix4f matrix{ 1.f };
	matrix[0][3] = -position[0];
	matrix[1][3] = -position[1];
	matrix[2][3] = -position[2];
	return matrix;
}

