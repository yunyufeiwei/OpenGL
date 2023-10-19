#version 430

layout (triangles) in;

//来自顶点着色器的输入
in vec3 varyingNormal[];
in vec3 varyingLightDir[];
in vec3 varyingHalfVector[];

//输出给光栅着色器然后到片段着色器
out vec3 varyingNormalG;
out vec3 varyingLightDirG;
out vec3 varyingHalfVectorG;

layout(triangle_strip, max_vertices=3) out;

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
uniform int enableLighting;

void main(void)
{
	//对三角形3个顶点法向量取平均值，得到三角形的曲面法向量
	vec4 triangleNormal = vec4(((varyingNormal[0]+varyingNormal[1]+varyingNormal[2])/3.0),1.0);

	//将三个顶点都沿法向量移动
	for(int i = 0 ; i<3 ; i++)
	{
		gl_Position = proj_matrix * (gl_in[i].gl_Position + normalize(triangleNormal) * 0.4);
		varyingNormalG = varyingNormal[i];
		varyingLightDirG = varyingLightDir[i];
		varyingHalfVectorG = varyingHalfVector[i];
		EmitVertex();
	}
	EndPrimitive();
}