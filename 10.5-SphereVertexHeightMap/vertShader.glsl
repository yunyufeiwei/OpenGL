#version 430

layout (location = 0) in vec3 vertPos;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 vertNormal;

out vec2 tc;

uniform mat4 mv_matrix;
uniform mat4 proj_matrix;

layout (binding=0) uniform sampler2D t;		//绑定普通纹理图
layout (binding=1) uniform sampler2D h;		//绑定高度图


void main(void)
{
	//采样高度图
	vec4 heightMAP = texture(h,texCoord);
	//P是高度图所改变的顶点位置
	vec4 p = vec4(vertPos,1.0f) + vec4((vertNormal * (heightMAP.r/8.0f)) , 1.0f);
	tc = texCoord;

	gl_Position = proj_matrix * mv_matrix * p;
}
