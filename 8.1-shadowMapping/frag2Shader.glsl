#version 430

in vec3 varyingVertPos;
in vec3 varyingNormal;
in vec3 varyingLightDir;
in vec3 varyingHalfVec;
in vec4 shadow_coord;

out vec4 Color;					//片段着色器输出的颜色

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
uniform mat4 norm_matrix;	//用法线来变换向量
uniform mat4 shadowMVP;

layout(binding=0)uniform sampler2DShadow shadowTex;

void main(void)
{
	vec3 L = normalize(varyingLightDir);
	vec3 N = normalize(varyingNormal);
	vec3 V = normalize(-varyingVertPos);
	vec3 H = normalize(L + V);

	//textureProj()函数输出0.0或1.0，如果处于阴影除则为0，否则为1
	float inShadow = textureProj(shadowTex , shadow_coord);

	Color = globalAmbient * material.ambient + light.ambient * material.ambient;
	if(inShadow != 0.0)
	{
		Color += light.diffuse * material.diffuse * max(dot(L,N),0.0) + light.specular * material.specular * pow(max(dot(N,H),0.0), material.shininess);
	}
}