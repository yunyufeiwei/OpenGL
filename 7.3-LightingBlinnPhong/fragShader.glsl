#version 430
in vec3 varyingVertPos;
in vec3 varyingNormal;
in vec3 varyingLightDir;

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
	vec3 L = normalize(varyingLightDir);
	vec3 N = normalize(varyingNormal);
	vec3 V = normalize(-varyingVertPos);
	vec3 H = normalize(L + V);

	//vec3 R = normalize(reflect(-L,N));//计算光照向量基于N的反射向量
	float cosTheta = dot(L,N);		//计算光照与平面法向量间的夹角
	float cosPhi = dot(N,H);		//计算视觉向量与反射光向量的角度

	vec3 ambient = ((globalAmbient * material.ambient) + (light.ambient * material.ambient)).xyz;
	vec3 diffuse = light.diffuse.xyz * material.diffuse.xyz * max(cosTheta,0.0);
	vec3 specular = light.specular.xyz * material.specular.xyz * pow(max(cosPhi,0.0), material.shininess);
	
	Color = vec4((ambient + diffuse + specular), 1.0);

}