#version 430

layout (location = 0) in vec3 position;

//原始模型的顶点将被用于纹理坐标
out vec3 originalPosition;

uniform mat4 mv_matrix;
uniform mat4 proj_matrix;

layout (binding=0) uniform sampler3D s;

void main(void)
{	originalPosition = position;		//将原始模型坐标传递，用作3D纹理坐标
	gl_Position = proj_matrix * mv_matrix * vec4(position,1.0);
}