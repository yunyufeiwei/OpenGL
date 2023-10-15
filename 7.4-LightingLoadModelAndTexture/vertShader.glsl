#version 430

layout(location = 0) in vec3 vertPos;
layout(location = 1) in vec3 vertNormal;
layout(location = 2) in vec2 texCoord;
out vec3 varyingNormal;
out vec3 varyingLightDir;
out vec3 varyingVertPos;
out vec2 tc;

layout(binding=0) uniform sampler2D samp;

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


void main(void)
{
	vec4 color;

	varyingVertPos = (mv_matrix * vec4(vertPos , 1.0)).xyz;
	varyingNormal = (norm_matrix * vec4(vertNormal , 1.0)).xyz;
	varyingLightDir = light.position - varyingVertPos;
	tc = texCoord;

	gl_Position = proj_matrix * mv_matrix * vec4(vertPos,1.0f);		//将顶点位置输出发送到片段着色器
}