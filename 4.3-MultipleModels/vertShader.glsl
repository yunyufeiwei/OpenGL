#version 430

layout(location = 0) in vec3 position;

uniform mat4 mv_matrix;
uniform mat4 proj_matrix;

//顶点颜色
out vec4 varyingColor;

void main()
{
	//这里的顶点要使用齐次坐标系vec4(position , 1.0)
	gl_Position = proj_matrix * mv_matrix * vec4(position , 1.0);
	varyingColor = vec4(position , 1.0) * 0.5 + vec4(0.5 , 0.5 , 0.5 , 0.5);
}