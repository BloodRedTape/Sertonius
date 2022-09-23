#version 440 core

layout(location = 0)in vec3 a_Position;
layout(location = 1)in vec3 a_Normal;
layout(location = 2)in vec3 a_Color;

void main(){
	gl_Position = vec4(a_Position.xyz, 1.0);
}