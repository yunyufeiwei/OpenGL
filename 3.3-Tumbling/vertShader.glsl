#version 430

layout(location = 0) in vec3 position;

uniform mat4 m_matrix;
uniform mat4 v_matrix;
uniform mat4 proj_matrix;
uniform float tf;				//���ڶ����ͷ����������ʱ������

out vec4 varyingColor;

mat4 buildRotateX(float rad);
mat4 buildRotateY(float rad);
mat4 buildRotateZ(float rad);
mat4 buildTranslate(float x ,float y ,float z);

void main(void)
{
//	//when 10000 instances
//	float x = gl_InstanceID + tf;
//	float a = sin(203.0 * x/8000.0) * 403.0;	
//	float b = cos(301.0 * x/4001.0) * 401.0;
//	float c = sin(400.0 * x/6003.0) * 405.0;

	//when 24 instances
	float i = gl_InstanceID + tf;		//ȡֵ����ʱ�����ӣ�����ÿ��������ʾ��Ҳ���ǲ�ͬ��
	float a = sin(0.35 * i) * 8.0f;
	float b = sin(0.52 * i) * 8.0f;
	float c = sin(0.78 * i) * 8.0f;

	//������ת��ƽ�ƾ��󣬽���Ӧ���뵱ǰ�������ģ�;���
	mat4 localRotX = buildRotateX(1000 * i);
	mat4 localRotY = buildRotateY(1000 * i);
	mat4 localRotZ = buildRotateZ(1000 * i);
	mat4 localTrans = buildTranslate(a,b,c);

	//����ģ�;���ģ��-��ͼ����
	mat4 newM_matrix = m_matrix * localTrans * localRotX * localRotY * localRotZ;
	mat4 mv_matrix = v_matrix * newM_matrix;

	gl_Position = proj_matrix * mv_matrix * vec4(position , 1.0);
	varyingColor = vec4(position , 1.0) * 0.5 + vec4(0.5 , 0.5 , 0.5 , 0.5);

}

mat4 buildTranslate(float x,float y , float z)
{
	mat4 trans = mat4(	1.0 , 0.0 , 0.0 , 0.0,
						0.0 , 1.0 , 0.0 , 0.0,
						0.0 , 0.0 , 1.0 , 0.0,
						x	, y   ,	z	, 1);
	return trans;
}

//  rotation around the X axis
mat4 buildRotateX(float rad)
{	mat4 xrot = mat4(	1.0 , 0.0	  , 0.0		 , 0.0,
						0.0 , cos(rad), -sin(rad), 0.0,
						0.0 , sin(rad), cos(rad) , 0.0,
						0.0 , 0.0	  , 0.0		 , 1.0 );
	return xrot;
}

//  rotation around the Y axis
mat4 buildRotateY(float rad)
{	mat4 yrot = mat4(	cos(rad), 0.0 , sin(rad), 0.0,
						0.0		, 1.0 , 0.0		, 0.0,
					   -sin(rad), 0.0 , cos(rad), 0.0,
						0.0		, 0.0 , 0.0		, 1.0 );
	return yrot;
}

//  rotation around the Z axis
mat4 buildRotateZ(float rad)
{	mat4 zrot = mat4(	cos(rad), sin(rad)	, 0.0	, 0.0,
					   -sin(rad), cos(rad)	, 0.0	, 0.0,
						0.0		, 0.0		, 1.0	, 0.0,
						0.0		, 0.0		, 0.0	, 1.0 );
	return zrot;
}
