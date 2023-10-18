#version 430

in vec3 varyingVertPos;
in vec3 varyingNormal;
in vec3 varyingLightDir;
in vec3 varyingHalfVec;
in vec4 shadow_coord;

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
uniform mat4 shadowMVP;

layout(binding=0)uniform sampler2DShadow shadowTex;

float lookup(float x , float y)
{
	float t = textureProj(shadowTex,shadow_coord + vec4(x * 0.001 * shadow_coord.w , y * 0.001 * shadow_coord.w , -0.01 , 0.0));		//第三个参数-0.01用于消除阴影痤疮的参数

	return t;
};

void main(void)
{
	float shadowFactor = 0.0;

	vec3 L = normalize(varyingLightDir);
	vec3 N = normalize(varyingNormal);
	vec3 V = normalize(-varyingVertPos);
	vec3 H = normalize(L + V);

	//textureProj()函数输出0.0或1.0，如果处于阴影除则为0，否则为1
	//float inShadow = textureProj(shadowTex , shadow_coord);

	//此部分生成一个4采样抖动的柔和阴影
	//float swidth = 2.5;
	//根据管glFragCoord mod 2 生成4采样模式中的一个
	/*vec2 offset = mod(floor(gl_FragCoord.xy),2.0 * swidth);
	shadowFactor += lookup(-1.5 * swidth + offset.x , 1.5 * swidth - offset.y);
	shadowFactor += lookup(-1.5 * swidth + offset.x , -0.5 * swidth - offset.y);
	shadowFactor += lookup( 0.5 * swidth + offset.x , 1.5 * swidth - offset.y);
	shadowFactor += lookup( 0.5 * swidth + offset.x , -0.5 * swidth - offset.y); 
	shadowFactor = shadowFactor/4.0;*/

	//此部分生成一个64采样的高分辨率柔和阴影
	float swidth = 2.5;			//调整阴影的扩散量
	float endp = swidth * 3.0 + swidth/2.0;
	for (float m=-endp ; m<=endp ; m=m+swidth)
	{	for (float n=-endp ; n<=endp ; n=n+swidth)
		{	shadowFactor += lookup(m,n);
	}	}
	shadowFactor = shadowFactor / 64.0;

	vec4 shadowColor = globalAmbient * material.ambient + light.ambient * material.ambient;
	vec4 lightedColor = light.diffuse * material.diffuse * max(dot(N,L) , 0.0) + light.specular * material.specular * pow(max(dot(N,H),0.0) , material.shininess * 3.0);

	Color = vec4((shadowColor.xyz + shadowFactor * lightedColor.xyz) , 1.0);
}