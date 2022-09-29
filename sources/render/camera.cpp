#include "render/camera.hpp"
#include <core/math/functions.hpp>
#include <core/math/transform.hpp>
#include <core/print.hpp>

Matrix4f Camera::MakeProjectionMatrix()const{
	Matrix4f swap_axis {
		{1.f, 0.f, 0.f, 0.f},
		{0.f, 0.f,-1.f, 0.f},
		{0.f, 1.f, 0.f, 0.f},
		{0.f, 0.f, 0.f, 1.f}
	};

	float fov = Math::Tan(FOV / 2.f);
	float f = Far;
	float n = Near;
#if 1
	Matrix4f projection{ 
		{1.f/(fov * Aspect),	0.f,		0.f,				0.f},
		{0.f,				    1.f/fov,	0.f,				0.f},
		{0.f,			 	    0.f,		n/(f - n),			f*n/(f - n)},
		{0.f,				    0.f,		-1.f,				0.f}
	};
#else
	Matrix4f projection{
		{1.f/2, 0.f,   0.f,    0.f},
		{0.f,   1.f/2, 0.f,    0.f},
		{0.f,   0.f,   1.f/20, 0.f},
		{0.f,   0.f,   0.f,    1.f}
	};
#endif
	return projection * swap_axis;
}

Matrix4f Camera::MakeViewMatrix(const Vector3f& position, const Vector3f& rotation) const{
	return Math::Rotate<float>(Math::Rad(-rotation)) * Math::Translate(-position); 
}

