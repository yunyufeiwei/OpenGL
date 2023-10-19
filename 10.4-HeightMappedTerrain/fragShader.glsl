#version 430

in vec3 varyingLightDir;
in vec3 varyingVertPos;
in vec3 varyingNormal;
in vec3 varyingTangent;
in vec2 tc;				//将顶点着色器中的纹理坐标传入到片段着色器

out vec4 fragColor;

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

uniform vec4 globalAmbient;
uniform mat4 mv_matrix;
uniform mat4 proj_matrix;
uniform mat4 norm_matrix;
uniform PositionalLight light;
uniform Material material;

//法线贴图修正函数
vec3 calcNewNormal()
{
	//传入的法线和切线信息
	vec3 normal = normalize(varyingNormal);
	vec3 tangent = normalize(varyingTangent);

	tangent = normalize(tangent - dot(tangent, normal) * normal);
	vec3 bitangent = cross(tangent, normal);
	mat3 tbn = mat3(tangent, bitangent, normal);			//构造TBN矩阵

	vec3 BumpMapNormal = texture(n,tc).xyz;
	BumpMapNormal = BumpMapNormal * 2.0 - 1.0;
	vec3 newNormal = tbn * BumpMapNormal;
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

	fragColor = texture(t,tc) + vec4((ambient + diffuse + specular) , 1.0);
}
