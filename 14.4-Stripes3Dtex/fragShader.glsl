#version 430

//接受原始模型坐标，用作3D纹理坐标
in vec3 originalPosition;
out vec4 fragColor;

uniform mat4 mv_matrix;	 
uniform mat4 proj_matrix;

layout (binding=0) uniform sampler3D s;

void main(void)
{	
	//顶点范围为[-1.1],纹理坐标范围为[0,1]
	fragColor = texture(s, originalPosition/2.0+.5);
}
