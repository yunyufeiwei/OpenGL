#version 430

//in�ؼ��ֱ�ʾ����������Խ���ӻ������н�����ֵ
layout(location = 0) in vec3 position;

uniform mat4 mv_matrix;
uniform mat4 proj_matrix;

void main()
{
	//����Ķ���Ҫʹ���������ϵvec4(position , 1.0)
	gl_Position = proj_matrix * mv_matrix * vec4(position , 1.0);
}