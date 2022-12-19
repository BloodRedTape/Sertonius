#version 440 core

layout(location = 0)in vec3 a_ModelPosition;
layout(location = 1)in vec3 a_Normal;
layout(location = 2)in vec3 a_Color;

layout(location = 0)out vec4 v_Albedo;
layout(location = 1)out vec4 v_Normal;
layout(location = 2)out vec4 v_WorldPosition;
layout(location = 3)out vec4 v_Material;

layout(binding = 0, row_major)uniform Camera{
	mat4 u_View;
	mat4 u_Projection;
};

layout(binding = 1, row_major)uniform Model{
	mat4 u_Model;
};
void main(){
	gl_Position = u_Projection * u_View * u_Model * vec4(a_ModelPosition, 1.0);
	v_Albedo = vec4(a_Color, 1);
	v_Normal = vec4(a_Normal, 1);
	v_WorldPosition = vec4(a_ModelPosition, 1);
	v_Material = vec4(0);
}