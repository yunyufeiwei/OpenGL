#version 430

//从顶点着色器中输入的颜色
in vec4 varyingColor;
//片段着色器输出的颜色
out vec4 color;

uniform mat4 mv_matrix;
uniform mat4 proj_matrix;

void main()
{
//	color = vec4(1.0 , 0.0 , 0.0 , 1.0);
	color = varyingColor;
}