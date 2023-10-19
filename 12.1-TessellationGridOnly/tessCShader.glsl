//曲面细分控制着色器
#version 430

uniform mat4 mvp_matrix;

layout (vertices = 1) out;

void main(void)
{
	gl_TessLevelOuter[0] = 6;		//网格体外部的细分段数
	gl_TessLevelOuter[2] = 6;
	gl_TessLevelOuter[1] = 6;
	gl_TessLevelOuter[3] = 6;
	gl_TessLevelInner[0] = 24;		//网格体内部的细分段数
	gl_TessLevelInner[1] = 24;
}