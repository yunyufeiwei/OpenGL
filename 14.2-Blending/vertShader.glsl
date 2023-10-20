#version 430

layout (location =0) in vec3 vertPos;
layout (location =2) in vec3 vertNormal;

out vec3 vNormal;
out vec3 vLightDir;
out vec3 vVertPos;
out vec3 vHalfVec;

layout(binding = 0) uniform sampler2D t;
layout(binding = 0) uniform sampler2D h;

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
uniform mat4 mv_matrix;
uniform mat4 proj_matrix;
uniform mat4 norm_matrix;
uniform PositionalLight light;
uniform Material material;

uniform float alpha;
uniform float flipNormal;

void main(void)
{
	//输出顶点位置到光栅器进行插值
	vVertPos = (mv_matrix * vec4(vertPos,1.0)).xyz;
	vLightDir = light.position - vVertPos;

	vNormal = (norm_matrix * vec4(vertNormal,1.0)).xyz;
	if (flipNormal < 0) vNormal = -vNormal;

	vHalfVec = (vLightDir-vVertPos).xyz;

	gl_Position = proj_matrix * mv_matrix * vec4(vertPos,1.0);
}
