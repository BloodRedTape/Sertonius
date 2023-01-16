
#include "common.glsl"

layout(location = 0)in vec3 v_Normal;
layout(location = 1)in vec3 v_Tangent;
layout(location = 2)in vec4 v_WorldPosition;
layout(location = 3)in vec4 v_Material;
layout(location = 4)in vec2 v_UV;

layout(location = 0)out vec4 f_Albedo;
layout(location = 1)out vec4 f_Normal;
layout(location = 2)out vec4 f_WorldPosition;
layout(location = 3)out vec4 f_Material;

layout(binding = 2, row_major)uniform Material {
	vec3 u_ColorValue;
};

layout(binding = 3)uniform sampler2D u_ColorTexture;
layout(binding = 4)uniform sampler2D u_NormalTexture;

vec3 CalculateNormal() {
	vec3 normal_texture = UnpackNormal(texture(u_NormalTexture, v_UV).xyz);

	mat3 tbn = mat3(
		v_Tangent,
		cross(v_Normal, v_Tangent),
		v_Normal
	);

	return tbn * normal_texture;
}
vec4 CalculateColor() {
	return vec4(u_ColorValue.xyz, 1.0) * texture(u_ColorTexture, v_UV);
}

void main(){
	f_Albedo = CalculateColor();
	f_Normal = vec4(PackNormal(CalculateNormal().xyz), 1.0);
	f_WorldPosition = v_WorldPosition;
	f_Material = v_Material;
}