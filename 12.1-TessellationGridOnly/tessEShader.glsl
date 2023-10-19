//曲面细分评估着色器
#version 430

uniform mat4 mvp_matrix;

layout (quads, equal_spacing, ccw) in;

void main(void)
{
	float u = gl_TessCoord.x;
	float v = gl_TessCoord.y;
	gl_Position = mvp_matrix * vec4(u,0,v,1);
}