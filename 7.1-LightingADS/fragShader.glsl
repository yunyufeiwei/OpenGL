#version 430
in  vec4 varyingColor;			//�Ӷ�����ɫ�����������ɫ
out vec4 Color;					//Ƭ����ɫ���������ɫ

struct PositionalLight
{
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	vec3 position;
};

struct Material
{
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	float shininess;
};

uniform vec4 globalAmbient;
uniform PositionalLight light;
uniform Material material;
uniform mat4 mv_matrix;
uniform mat4 proj_matrix;
uniform mat4 norm_matrix;	//�÷������任����

void main(void)
{
	Color = varyingColor;
}