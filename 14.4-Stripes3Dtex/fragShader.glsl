#version 430

//����ԭʼģ�����꣬����3D��������
in vec3 originalPosition;
out vec4 fragColor;

uniform mat4 mv_matrix;	 
uniform mat4 proj_matrix;

layout (binding=0) uniform sampler3D s;

void main(void)
{	
	//���㷶ΧΪ[-1.1],�������귶ΧΪ[0,1]
	fragColor = texture(s, originalPosition/2.0+.5);
}
