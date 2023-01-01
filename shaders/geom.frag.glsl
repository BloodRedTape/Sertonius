#version 440 core

layout(location = 0)in vec4 v_Normal;
layout(location = 1)in vec4 v_WorldPosition;
layout(location = 2)in vec4 v_Material;
layout(location = 3)in vec2 v_UV;

layout(location = 0)out vec4 f_Albedo;
layout(location = 1)out vec4 f_Normal;
layout(location = 2)out vec4 f_WorldPosition;
layout(location = 3)out vec4 f_Material;

layout(binding = 2, row_major)uniform Material {
	vec3 u_ColorValue;
};

layout(binding = 3)uniform sampler2D u_ColorTexture;

void main(){
	f_Albedo = vec4(u_ColorValue.xyz, 1.0) * texture(u_ColorTexture, v_UV);
	f_Normal = v_Normal;
	f_WorldPosition = v_WorldPosition;
	f_Material = v_Material;
}