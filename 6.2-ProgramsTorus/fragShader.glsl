#version 430

//从顶点着色器中输入的颜色
in vec2 tc;

//片段着色器输出的颜色
out vec4 Color;

uniform mat4 mv_matrix;
uniform mat4 proj_matrix;

layout(binding=0) uniform sampler2D samp;

void main(void)
{
	Color = texture(samp,tc);
}