#version 430

//�Ӷ�����ɫ�����������ɫ
in vec4 varyingColor;
//Ƭ����ɫ���������ɫ
out vec4 color;

uniform mat4 mv_matrix;
uniform mat4 proj_matrix;

void main()
{
//	color = vec4(1.0 , 0.0 , 0.0 , 1.0);
	color = varyingColor;
}