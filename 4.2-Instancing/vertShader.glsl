#version 430

layout(location = 0) in vec3 position;

//分开模型矩阵与视图矩阵
uniform mat4 v_matrix;
//投影矩阵
uniform mat4 proj_matrix;
uniform float tf;			//用于动画和放置立方的时间因子

//顶点颜色
out vec4 varyingColor;

//矩阵变换工具函数的声明
//GLSL要求函数先声明后调用（回调）
mat4 buildRotateX(float rad);
mat4 buildRotateY(float rad);
mat4 buildRotateZ(float rad);
mat4 buildTranslate(float x, float y, float z);

void main(void)
{
	float  x = gl_InstanceID + tf;

	//当实例化100000个对象时
	float a = sin(203.0 * x/8000.0) * 403.0;
	float b = cos(301.0 * x/4001.0) * 401.0;
	float c = sin(400.0 * x/6003.0) * 405.0;

	//当实例化24个对象时
	//float a = sin(0.35 * x) * 8.0;
	//float b = cos(0.52 * x) * 8.0;
	//float c = sin(0.70 * x) * 8.0;

	//构建旋转矩阵和平移矩阵，应用于当前立方体的模型矩阵
	mat4 localRotX = buildRotateX(1.75 * x);
	mat4 localRotY = buildRotateY(1.75 * x);
	mat4 localRotZ = buildRotateZ(1.75 * x);
	mat4 localTrans = buildTranslate(a,b,c);

	mat4 newM_matrix = localTrans * localRotX * localRotY * localRotZ;
	mat4 mv_matrix = v_matrix * newM_matrix;

	gl_Position = proj_matrix * mv_matrix * vec4(position,1.0);
	varyingColor = vec4(position , 1.0) * 0.5 + vec4(0.5 , 0.5 , 0.5 , 0.5);
}

//平移矩阵
mat4 buildTranslate(float x,float y,float z)
{
	mat4 trans = mat4(1.0, 0.0, 0.0, 0.0,
					  0.0, 1.0, 0.0, 0.0,
					  0.0, 0.0, 1.0, 0.0,
					  x, y, z, 1.0 );
	return trans;
}

//绕X轴旋转矩阵
mat4 buildRotateX(float rad)
{
	mat4 xrot = mat4(1.0, 0.0, 0.0, 0.0,
					0.0, cos(rad), -sin(rad), 0.0,
					0.0, sin(rad), cos(rad), 0.0,
					0.0, 0.0, 0.0, 1.0 );
	return xrot;
}
//绕Y轴旋转矩阵
mat4 buildRotateY(float rad)
{
	mat4 yrot = mat4(cos(rad), 0.0, sin(rad), 0.0,
					0.0, 1.0, 0.0, 0.0,
					-sin(rad), 0.0, cos(rad), 0.0,
					0.0, 0.0, 0.0, 1.0 );
	return yrot;
}
//绕Z轴旋转矩阵
mat4 buildRotateZ(float rad)
{	mat4 zrot = mat4(cos(rad), sin(rad), 0.0, 0.0,
					-sin(rad), cos(rad), 0.0, 0.0,
					0.0, 0.0, 1.0, 0.0,
					0.0, 0.0, 0.0, 1.0 );
	return zrot;
}




