layout(location = 0)in vec2 a_VertexPosition;
layout(location = 1)in vec3 a_LightPosition;
layout(location = 2)in vec3 a_LightColor;
layout(location = 3)in float a_LightRadius;

layout(location = 0)out vec3 v_LightColor;
layout(location = 1)out vec3 v_LightPosition;
layout(location = 2)out float v_LightRadius;

void main() {
	gl_Position = vec4(a_VertexPosition.xy, 0.0, 1.0);
	v_LightPosition = a_LightPosition;
	v_LightColor = a_LightColor;
	v_LightRadius = a_LightRadius;
}