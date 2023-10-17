#version 430

layout(location = 0) in vec3 vertPos;
layout(location = 1) in vec3 vertNormal;

out vec3 varyingNormal , varyingLightDir , varyingVertPos , varyingHalfVec;
out vec4 shadow_coord;				//阴影纹理

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

layout(binding=0) uniform sampler2DShadow shadowTex;


void main(void)
{
	vec4 color;

	varyingVertPos = (mv_matrix * vec4(vertPos , 1.0)).xyz;
	varyingLightDir = light.position - varyingVertPos;
	varyingNormal = (norm_matrix * vec4(vertNormal , 1.0)).xyz;
	varyingHalfVec = (varyingLightDir -  varyingVertPos).xyz;

	shadow_coord = shadowMVP * vec4(vertPos , 1.0);

	gl_Position = proj_matrix * mv_matrix * vec4(vertPos,1.0f);		//将顶点位置输出发送到片段着色器
}