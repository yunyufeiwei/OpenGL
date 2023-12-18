#version 430

layout (location = 0) in vec3 vertPos;
layout (location = 1) in vec3 vertNormal;
out vec3 varyingNormal;
out vec3 varyingLightDir;
out vec3 varyingVertPos;
out vec3 varyingHalfVector;

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

uniform vec4 globalAmbient;
uniform PositionalLight light;
uniform Material material;
uniform mat4 mv_matrix;
uniform mat4 proj_matrix;
uniform mat4 norm_matrix;
uniform int flipNormal;

//�ü����������ֱ��Ӧ��xyz
vec4 clip_plane = vec4(0.0, 0.0, -1.0f, 0.5);

void main(void)
{	varyingVertPos = (mv_matrix * vec4(vertPos,1.0)).xyz;
	varyingLightDir = light.position - varyingVertPos;
	varyingNormal = (norm_matrix * vec4(vertNormal,1.0)).xyz;
	
	if (flipNormal==1) varyingNormal = -varyingNormal;
	
	varyingHalfVector = normalize(normalize(varyingLightDir) + normalize(-varyingVertPos)).xyz;
	
	//ax+by+cz+d = 0,�����ֱ��ʾ��ֱ��ƽ�������(a,b,c)���Լ���ԭ�㵽ƽ��ľ���d������ʹ��vector4�ڶ�����ɫ����ָ��������ƽ��vec4 clip_plane = vec4(0.0f,0.0f,-1.0f,0.2)
	gl_ClipDistance[0] = dot(clip_plane.xyz, vertPos) + clip_plane.w;

	gl_Position = proj_matrix * mv_matrix * vec4(vertPos,1.0);
}