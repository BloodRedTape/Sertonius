#version 440 core

layout(local_size_x = 16, local_size_y = 16, local_size_z = 1) in;

layout(binding = 0)uniform sampler2D u_Albedo;
layout(binding = 1)uniform sampler2D u_Normal;
layout(binding = 2)uniform sampler2D u_WorldPosition;
layout(binding = 3)uniform sampler2D u_Material;
layout(binding = 4)uniform sampler2D u_Lighting;

layout(binding = 0 + 5, rgba8)uniform image2D u_PresentTarget;

void main() {
	ivec2 outCoord = ivec2(gl_GlobalInvocationID.xy);
	vec2 inCoord = vec2(float(outCoord.x)/(gl_NumWorkGroups.x*gl_WorkGroupSize.x), float(outCoord.y) / (gl_NumWorkGroups.y * gl_WorkGroupSize.y));
	
	float ambient = 0.3;

	vec4 lighting = texture(u_Lighting, inCoord) + ambient;
	vec4 albedo = texture(u_Albedo, inCoord);
	imageStore(u_PresentTarget, outCoord, lighting * albedo);
}