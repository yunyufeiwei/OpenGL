#version 430

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 texCoord;

//������ɫ
out vec2 tc;

uniform mat4 mv_matrix;
uniform mat4 proj_matrix;

layout(binding=0) uniform sampler2D samp;

void main(void)
{
	//����Ķ���Ҫʹ���������ϵvec4(position , 1.0)
	gl_Position = proj_matrix * mv_matrix * vec4(pos , 1.0);
	tc = texCoord;
}