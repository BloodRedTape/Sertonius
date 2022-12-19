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
		{0.f,			 	    0.f,		-(f + n)/(f - n),	-2*f*n/(f - n)},
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

	Matrix4f invert_z{
		{1.f, 0.f, 0.f, 0.f},
		{0.f, 1.f, 0.f, 0.f},
		{0.f, 0.f,-1.f, 0.f},
		{0.f, 0.f, 0.f, 1.f}
	};
	return projection * invert_z * swap_axis;
}

