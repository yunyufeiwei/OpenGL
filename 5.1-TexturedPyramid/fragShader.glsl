#version 430

//�Ӷ�����ɫ�����������ɫ
in vec2 tc;

//Ƭ����ɫ���������ɫ
out vec4 Color;

uniform mat4 mv_matrix;
uniform mat4 proj_matrix;

layout(binding=0) uniform sampler2D samp;

void main(void)
{
	Color = texture(samp,tc);
}