#version 430

layout(location = 0) in vec3 vertPos;
layout(location = 1) in vec3 vertNormal;
out vec4 varyingColor;

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

	vec4 P = mv_matrix * vec4(vertPos,1.0f);						//将顶点位置转换到视图空间
	vec3 N = normalize(norm_matrix * vec4(vertNormal,1.0f)).xyz;	//将法线向量转换到视图空间
	vec3 L = normalize(light.position - P.xyz);						//计算视图空间下的光照向量（从顶点到光源）	

	vec3 V = normalize(-P.xyz);										//视向量等于视觉空间中的负顶点为止

	vec3 R = reflect(-L,N);											//反射向量是-L的相对于表面向量N的镜像

	//计算环境光、漫反射、镜面反射
	vec3 ambient = ((globalAmbient * material.ambient) + (light.ambient * material.ambient)).xyz;
	vec3 diffuse = light.diffuse.xyz * material.diffuse.xyz * max(dot(N,L),0.0f);
	vec3 specular= light.specular.xyz * material.specular.xyz * pow(max(dot(R,V),0.0f),material.shininess);

	varyingColor = vec4((ambient + diffuse + specular),1.0f);		//将颜色输出发送到片段着色器
	gl_Position = proj_matrix * mv_matrix * vec4(vertPos,1.0f);		//将顶点位置输出发送到片段着色器
}