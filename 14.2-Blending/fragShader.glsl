#version 430

in vec3 vNormal;
in vec3 vLightDir;
in vec3 vVertPos;
in vec3 vHalfVec;

out vec4 fragColor;


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
	vec3 L = normalize(vLightDir);
	vec3 N = normalize(vNormal);
	vec3 V = normalize(-vVertPos);
	vec3 H = normalize(vHalfVec);

	vec3 ambient = ((globalAmbient * material.ambient) + (light.ambient * material.ambient)).xyz;
	vec3 diffuse = light.diffuse.xyz * material.diffuse.xyz * max(dot(L,N),0.0);
	vec3 specular = light.specular.xyz * material.specular.xyz * pow(max(dot(H,N),0.0f), material.shininess);

	fragColor = vec4(ambient.xyz + diffuse.xyz +specular.xyz , alpha);
}