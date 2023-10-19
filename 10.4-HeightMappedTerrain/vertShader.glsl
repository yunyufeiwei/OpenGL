#version 430

layout (location = 0) in vec3 vertPos;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 vertNormal;
layout (location = 3) in vec3 vertTangent;

out vec3 varyingLightDir;
out vec3 varyingVertPos;
out vec3 varyingNormal;
out vec3 varyingTangent;
out vec2 tc;					//�����������

layout (binding = 0) uniform sampler2D t;		//����ͨ����ͼ
layout (binding = 1) uniform sampler2D n;		//�󶨷�������
layout (binding = 2) uniform sampler2D h;		//�󶨸߶�ͼ

struct PositionalLight
{	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	vec3 position;
};
struct Material
{	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	float shininess;
};

uniform mat4 mv_matrix;
uniform mat4 proj_matrix;
uniform mat4 norm_matrix;
uniform vec4 globalAmbient;
uniform PositionalLight light;
uniform Material material;

void main(void)
{
	varyingNormal  = (norm_matrix * vec4(vertNormal , 1.0f)).xyz;
	varyingTangent = (norm_matrix * vec4(vertTangent, 1.0f)).xyz;

	vec4 heightMAP = texture(h,texCoord);		//�����߶�ͼ	

	vec4 P1 = vec4(vertPos , 1.0f);
	vec4 P2 = vec4(vertNormal * (heightMAP.r / 15.0f) , 1.0);	//ʹ�ø߶�ͼ��ͨ�����߷�����ж���ƫ��
	vec4 P  = vec4((P1.xyz + P2.xyz) , 1.0f);

	tc = texCoord;		//���������긳ֵ��tc���������뵽Ƭ����ɫ����

	gl_Position = proj_matrix * mv_matrix * P;
}
