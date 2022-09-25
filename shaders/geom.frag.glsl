#version 440 core

layout(location = 0)in vec4 v_Albedo;
layout(location = 1)in vec4 v_Normal;
layout(location = 2)in vec4 v_WorldPosition;
layout(location = 3)in vec4 v_Material;

layout(location = 0)out vec4 f_Albedo;
layout(location = 1)out vec4 f_Normal;
layout(location = 2)out vec4 f_WorldPosition;
layout(location = 3)out vec4 f_Material;

void main(){
	f_Albedo = v_Albedo;
	f_Normal = v_Normal;
	f_WorldPosition = v_WorldPosition;
	f_Material = v_Material;
}