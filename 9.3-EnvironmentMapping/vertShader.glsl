#version 430

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

uniform mat4 mv_matrix;
uniform mat4 proj_matrix;
uniform mat4 norm_matrix;

out vec3 VaryingNormal;
out vec3 VaryingVertPos;

layout(binding = 0) uniform samplerCube tex_map;

void main(void)
{
	VaryingVertPos = (mv_matrix * vec4(position , 1.0f)).xyz;
	VaryingNormal = (norm_matrix * vec4(position , 1.0f)).xyz;
	gl_Position = proj_matrix * mv_matrix * vec4(position,1.0f);
}
