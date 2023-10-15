#version 430
in  vec4 varyingColor;			//从顶点着色器中输入的颜色
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

void main(void)
{
	Color = varyingColor;
}