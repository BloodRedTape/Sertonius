#version 440 core

layout(location = 0)in vec3 a_ModelPosition;
layout(location = 1)in vec3 a_Normal;
layout(location = 2)in vec3 a_Color;

layout(location = 0)out vec4 v_Albedo;
layout(location = 1)out vec4 v_Normal;
layout(location = 2)out vec4 v_WorldPosition;
layout(location = 3)out vec4 v_Material;

void main(){
	gl_Position = vec4(a_ModelPosition, 1.0);
	v_Albedo = vec4(a_Color, 1);
	v_Normal = vec4(a_Normal, 1);
	v_WorldPosition = vec4(a_ModelPosition, 1);
	v_Material = vec4(0);
}