#version 430

in vec3 VaryingNormal;
in vec3 VaryingVertPos;

out vec4 fragColor;

uniform mat4 mv_matrix;
uniform mat4 proj_matrix;
uniform mat4 norm_matrix;

layout(binding = 0) uniform samplerCube tex_map;

void main(void)
{
	vec3 reflection = -reflect(normalize(-VaryingVertPos),normalize(VaryingNormal));
	fragColor = texture(tex_map,reflection);
}
