#include "common.glsl"

layout(location = 0)in flat vec3 v_LightColor;
layout(location = 1)in vec3 v_LightPosition;
layout(location = 2)in float v_LightRadius;

layout(location = 0)out vec4 f_Lighting;

layout(binding = 0)uniform sampler2D u_Positions;
layout(binding = 1)uniform sampler2D u_Normals;

vec3 Grayscale(vec3 color) {
	return vec3((color.r + color.g + color.b) / 3);
}

layout(binding = 2)uniform Camera {
	vec3 u_CameraPosition;
};

void main() {
	const vec2 UV = gl_FragCoord.xy / textureSize(u_Positions, 0);
	vec3 fragment_position = texture(u_Positions, UV).xyz;
	vec3 fragment_normal = normalize(UnpackNormal(texture(u_Normals, UV).xyz));
	
	float light_length = length(v_LightPosition - fragment_position);
	vec3 light_direction = normalize(v_LightPosition - fragment_position);

	float attenuation = 1 - clamp(light_length / v_LightRadius, 0, 1);

	float diffuse = dot(fragment_normal, light_direction) * attenuation;

	f_Lighting = vec4(v_LightColor * diffuse, 1.0);
}