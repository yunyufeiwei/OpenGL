#version 430

void main(void)
{
	//定义三个顶点索引，并指定位置
	if(gl_VertexID == 0) 
		gl_Position = vec4(0.25,-0.25,0,1.0);
	else if(gl_VertexID == 1) 
		gl_Position = vec4(-0.25,-0.25,0,1.0);
	else 
		gl_Position = vec4(0,0.25,0,1.0);
}