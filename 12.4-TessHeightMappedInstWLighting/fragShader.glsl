#version 430

in vec2 tes_out;
out vec4 fragColor;
uniform mat4 mvp_matrix;

in vec3 varyingVertPos;
in vec3 varyingLightDir;

layout (binding=0) uniform sampler2D tex_color;
layout (binding = 1) uniform sampler2D tex_height;
layout (binding = 2) uniform sampler2D tex_normal;

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

vec3 calcNewNormal()
{
	vec3 normal = vec3(0,1,0);
	vec3 tangent = vec3(1,0,0);
	tangent = normalize(tangent - dot(tangent, normal) * normal);
	vec3 bitangent = cross(tangent, normal);
	mat3 tbn = mat3(tangent, bitangent, normal);
	vec3 retrievedNormal = texture(tex_normal,tes_out).xyz;
	retrievedNormal = retrievedNormal * 2.0 - 1.0;
	vec3 newNormal = tbn * retrievedNormal;
	newNormal = normalize(newNormal);
	return newNormal;
}

void main(void)
{
	vec3 L = normalize(varyingLightDir);
	vec3 V = normalize(-varyingVertPos);
	vec3 N = calcNewNormal();

	float cosTheta = dot(N,L);		//灯光和法线的夹角
	vec3 R = normalize(reflect(-L,N));	//反射向量
	float cosPhi = dot(V,R);		//实现和法线方向的夹角

	vec3 ambient = ((globalAmbient * material.ambient) + (light.ambient * material.ambient)).xyz;
	vec3 diffuse = light.diffuse.xyz * material.diffuse.xyz * max(cosTheta,0.0);
	vec3 specular = light.specular.xyz * material.specular.xyz * pow(max(cosPhi,0.0), material.shininess);

	fragColor = texture(tex_color,tes_out) + vec4((ambient + diffuse + specular) , 1.0);
}