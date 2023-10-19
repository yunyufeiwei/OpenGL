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
	//移除图元
	if (mod(gl_PrimitiveIDIn,3)!=0)
	{
		for(int i = 0 ; i<3 ; i++)
		{
			gl_Position = proj_matrix * gl_in[i].gl_Position;
			varyingNormalG = varyingNormal[i];
			varyingLightDirG = varyingLightDir[i];
			varyingHalfVectorG = varyingHalfVector[i];
			EmitVertex();
		}
	}
	EndPrimitive();
}