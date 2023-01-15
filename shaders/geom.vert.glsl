#version 440 core

layout(location = 0)in vec3 a_ModelPosition;
layout(location = 1)in vec3 a_Normal;
layout(location = 2)in vec3 a_Tangent;
layout(location = 3)in vec2 a_UV;

layout(location = 0)out vec3 v_Normal;
layout(location = 1)out vec3 v_Tangent;
layout(location = 2)out vec4 v_WorldPosition;
layout(location = 3)out vec4 v_Material;
layout(location = 4)out vec2 v_UV;

layout(binding = 0, row_major)uniform Camera{
	mat4 u_View;
	mat4 u_Projection;
};

layout(binding = 1, row_major)uniform Model{
	mat4 u_Model;
};

vec3 HomoToNoHomo3(vec4 homo) {
	return vec3(homo.xyz / homo.w);
}

void main(){
	vec4 world_position = u_Model * vec4(a_ModelPosition, 1.0);
	gl_Position = u_Projection * u_View * world_position;
	v_Normal = normalize(mat3(u_Model) * a_Normal);
	v_Tangent = normalize(mat3(u_Model) * a_Tangent);
	v_WorldPosition = vec4(HomoToNoHomo3(world_position), 1.0);
	v_Material = vec4(0);
	v_UV = a_UV;
}