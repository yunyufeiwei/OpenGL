#version 430

layout (location = 0) in vec3 vertPos;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 vertNormal;
layout (location = 3) in vec3 vertTangent;

out vec3 varyingLightDir;
out vec3 varyingVertPos;
out vec3 varyingNormal;
out vec3 varyingTangent;
out vec2 tc;					//输出纹理坐标

layout (binding = 0) uniform sampler2D t;		//绑定普通纹理图
layout (binding = 1) uniform sampler2D n;		//绑定法线纹理
layout (binding = 2) uniform sampler2D h;		//绑定高度图

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

	vec4 heightMAP = texture(h,texCoord);		//采样高度图	

	vec4 P1 = vec4(vertPos , 1.0f);
	vec4 P2 = vec4(vertNormal * (heightMAP.r / 15.0f) , 1.0);	//使用高度图，通过法线方向进行顶点偏移
	vec4 P  = vec4((P1.xyz + P2.xyz) , 1.0f);

	tc = texCoord;		//将纹理坐标赋值给tc，后续传入到片段着色器中

	gl_Position = proj_matrix * mv_matrix * P;
}
