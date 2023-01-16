
vec3 UnpackNormal(vec3 normal) {
	return normal * 2.0 - 1.0;
}

vec3 PackNormal(vec3 normal) {
	return (normal + 1.0) / 2.0;
}