//曲面细分控制着色器
#version 430
in vec2 texcoord[];

out vec2 tcs_out[];

uniform mat4 mvp_matrix;

layout(binding = 0) uniform sampler2D tex_color;
layout (vertices = 16) out;

void main(void)
{
	int TL=32;						//细分等级
	if(gl_InvocationID == 0)
	{
		gl_TessLevelOuter[0] = TL;		//网格体外部的细分段数
		gl_TessLevelOuter[2] = TL;
		gl_TessLevelOuter[1] = TL;
		gl_TessLevelOuter[3] = TL;
		gl_TessLevelInner[0] = TL;		//网格体内部的细分段数
		gl_TessLevelInner[1] = TL;
	}
	tcs_out[gl_InvocationID] = texcoord[gl_InvocationID];
	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
	
}