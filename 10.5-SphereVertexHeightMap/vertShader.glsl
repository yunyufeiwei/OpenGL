#version 430

layout (location = 0) in vec3 vertPos;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 vertNormal;

out vec2 tc;

uniform mat4 mv_matrix;
uniform mat4 proj_matrix;

layout (binding=0) uniform sampler2D t;		//����ͨ����ͼ
layout (binding=1) uniform sampler2D h;		//�󶨸߶�ͼ


void main(void)
{
	//�����߶�ͼ
	vec4 heightMAP = texture(h,texCoord);
	//P�Ǹ߶�ͼ���ı�Ķ���λ��
	vec4 p = vec4(vertPos,1.0f) + vec4((vertNormal * (heightMAP.r/8.0f)) , 1.0f);
	tc = texCoord;

	gl_Position = proj_matrix * mv_matrix * p;
}
