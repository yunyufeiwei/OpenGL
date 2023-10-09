#version 430

layout(location = 0) in vec3 position;

//�ֿ�ģ�;�������ͼ����
uniform mat4 v_matrix;
//ͶӰ����
uniform mat4 proj_matrix;
uniform float tf;			//���ڶ����ͷ���������ʱ������

//������ɫ
out vec4 varyingColor;

//����任���ߺ���������
//GLSLҪ��������������ã��ص���
mat4 buildRotateX(float rad);
mat4 buildRotateY(float rad);
mat4 buildRotateZ(float rad);
mat4 buildTranslate(float x, float y, float z);

void main(void)
{
	float  x = gl_InstanceID + tf;

	//��ʵ����100000������ʱ
	float a = sin(203.0 * x/8000.0) * 403.0;
	float b = cos(301.0 * x/4001.0) * 401.0;
	float c = sin(400.0 * x/6003.0) * 405.0;

	//��ʵ����24������ʱ
	//float a = sin(0.35 * x) * 8.0;
	//float b = cos(0.52 * x) * 8.0;
	//float c = sin(0.70 * x) * 8.0;

	//������ת�����ƽ�ƾ���Ӧ���ڵ�ǰ�������ģ�;���
	mat4 localRotX = buildRotateX(1.75 * x);
	mat4 localRotY = buildRotateY(1.75 * x);
	mat4 localRotZ = buildRotateZ(1.75 * x);
	mat4 localTrans = buildTranslate(a,b,c);

	mat4 newM_matrix = localTrans * localRotX * localRotY * localRotZ;
	mat4 mv_matrix = v_matrix * newM_matrix;

	gl_Position = proj_matrix * mv_matrix * vec4(position,1.0);
	varyingColor = vec4(position , 1.0) * 0.5 + vec4(0.5 , 0.5 , 0.5 , 0.5);
}

//ƽ�ƾ���
mat4 buildTranslate(float x,float y,float z)
{
	mat4 trans = mat4(1.0, 0.0, 0.0, 0.0,
					  0.0, 1.0, 0.0, 0.0,
					  0.0, 0.0, 1.0, 0.0,
					  x, y, z, 1.0 );
	return trans;
}

//��X����ת����
mat4 buildRotateX(float rad)
{
	mat4 xrot = mat4(1.0, 0.0, 0.0, 0.0,
					0.0, cos(rad), -sin(rad), 0.0,
					0.0, sin(rad), cos(rad), 0.0,
					0.0, 0.0, 0.0, 1.0 );
	return xrot;
}
//��Y����ת����
mat4 buildRotateY(float rad)
{
	mat4 yrot = mat4(cos(rad), 0.0, sin(rad), 0.0,
					0.0, 1.0, 0.0, 0.0,
					-sin(rad), 0.0, cos(rad), 0.0,
					0.0, 0.0, 0.0, 1.0 );
	return yrot;
}
//��Z����ת����
mat4 buildRotateZ(float rad)
{	mat4 zrot = mat4(cos(rad), sin(rad), 0.0, 0.0,
					-sin(rad), cos(rad), 0.0, 0.0,
					0.0, 0.0, 1.0, 0.0,
					0.0, 0.0, 0.0, 1.0 );
	return zrot;
}




