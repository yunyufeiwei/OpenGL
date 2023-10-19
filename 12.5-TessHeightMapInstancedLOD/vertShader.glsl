#version 430

out vec2 texcoord;

uniform mat4 mvp_matrix;
layout (binding = 0) uniform sampler2D tex_color;
layout (binding = 1) uniform sampler2D tex_height;

void main(void)
{	
	vec2 patchTexCoords[] = vec2[] (vec2(0,0), vec2(1,0), vec2(0,1), vec2(1,1));
	
	//���ڵ�ǰ���ĸ�ʵ�������������ƫ����
	int x = gl_InstanceID % 64;
	int y = gl_InstanceID / 64;
	
	//�������걻�����64�������У�����һ����[0,1],��תY������
	texcoord = vec2( (x+patchTexCoords[gl_VertexID].x)/64.0,
			   (63-y+patchTexCoords[gl_VertexID].y)/64.0 );
	
	//����λ�ú�����������ͬ��ֻ������ȡֵ��Χ��-0.5��+0.5
	gl_Position = vec4(texcoord.x-0.5, 0.0, (1.0-texcoord.y)-0.5, 1.0);
}