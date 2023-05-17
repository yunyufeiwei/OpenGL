#version 430

layout(location = 0) in vec3 position;

uniform mat4 mv_matrix;
uniform mat4 proj_matrix;

void main()
{
	//这里的顶点要使用齐次坐标系vec4(position , 1.0)
	gl_Position = proj_matrix * mv_matrix * vec4(position , 1.0);
}