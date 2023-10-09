using namespace std;
#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<SOIL2/SOIL2.h>
#include<iostream>
#include<string>
#include<fstream>	
#include<cmath>
#include<vector>
#include<glm/glm.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include "Utils.h"

Utils::Utils(){}

//
string Utils::readShaderFile(const char* filePath)
{
	string content;
	ifstream fileStream(filePath, ios::in);
	string line = "";
	while (!fileStream.eof())			//EOF(filenumber)��������False��ֱ���ﵽ�ļ���ĩβ�����ﵽĩβ�ͷ���True��ִ��while�����ڵĲ���
	{
		getline(fileStream, line);
		content.append(line + "\n");
	}
	fileStream.close();
	return content;
}

//�쳵�����log����ʵ�֣����Ͳ�����
bool Utils::checkOpenGLError()
{
	bool foundError = false;		//����һ����������foundError
	int glErr = glGetError();		//glGetError��ȡ�������Ϣ����ָ����glErr����
	while (glErr != GL_NO_ERROR)	//GL_NO_ERROR,δ��¼�κδ��󣬴˷��ų�����ֻ��֤Ϊ�㣬��glErr�����ڣ���ζ�ŷ����˴���
	{
		cout << "glError:" << glErr << endl;		//���glError�Ĵ���log��ʾ
		foundError = true;							//�������˴���ʱ���޸�foundError��ֵΪ1
		glErr = glGetError();						//�����ȡ������Ϣ��ָ��
	}
	return foundError;
}

//��GLSL����ʧ��ʱ����ʾOpenGL��־����
void Utils::printShaderLog(GLuint shader)
{
	int len = 0;
	int chWrittn = 0;
	char* log;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
	if (len > 0)
	{
		log = (char*)malloc(len);
		glGetShaderInfoLog(shader, len, &chWrittn, log);
		cout << "Shader Info Log:" << log << endl;
		free(log);
	}
}

//׼��Shader
GLuint Utils::prepareShader(int shaderTYPE, const char* shaderPath)
{
	GLint shaderCompiled;
	//����һ������shaderStr������ȡ�ļ�(����/Ƭ����ɫ��)
	string shaderStr = readShaderFile(shaderPath);	
	const char* shaderSrc = shaderStr.c_str();
	//����һ������shaderRef��������ɫ��
	GLuint shaderRef = glCreateShader(shaderTYPE);		
	//����glShaderSource()���������ڽ�GLSL������ַ����������ɫ�������У�֮����ʹ��glCompilShader()�����������ɫ��
	//glShaderSource��Ҫ����4��������1-���������ɫ������2-��ɫ��Դ�����е��ַ������� 3-����Դ������ַ���ָ�� 4-��������
	glShaderSource(shaderRef, 1, &shaderSrc, NULL);
	//����shader
	glCompileShader(shaderRef);
	//����shader
	checkOpenGLError();
	glGetShaderiv(shaderRef, GL_COMPILE_STATUS, &shaderCompiled);
	if (shaderCompiled != 1)
	{
		if (shaderTYPE == 35633) cout << "Vertex ";
		if (shaderTYPE == 36488) cout << "Tess Control ";
		if (shaderTYPE == 36487) cout << "Tess Eval ";
		if (shaderTYPE == 36313) cout << "Geometry ";
		if (shaderTYPE == 35632) cout << "Fragment ";
		cout << "shader compilation error." << endl;
		printShaderLog(shaderRef);
	}
	return shaderRef;
}

//��GLSL������ɫ��ʧ��ʱ����ʾOpenGL��־����
void Utils::printProgramLog(int prog)
{
	int len = 0;
	int chWrittn = 0;
	char* log;
	glGetProgramiv(prog, len, &len);
	if (len > 0)
	{
		log = (char*)malloc(len);
		glGetProgramInfoLog(prog, len, &chWrittn, log);
		cout << "Program Info Log : " << log << endl;
		free(log);
	}
}

int Utils::finalizeShaderProgram(GLuint sprogram)
{
	GLint linked;
	glLinkProgram(sprogram);
	checkOpenGLError();
	glGetProgramiv(sprogram, GL_LINK_STATUS, &linked);
	if (linked != 1)
	{
		cout << "Linking failed ��" << endl;
		printProgramLog(sprogram);
	}
	return sprogram;
}

//��������Ƭ����ɫ������
GLuint Utils::createShaderProgram(const char *vp, const char *fp)
{
	//Ԥ���붥�㡢Ƭ�β���
	GLuint vShader = prepareShader(GL_VERTEX_SHADER, vp);
	GLuint fShader = prepareShader(GL_FRAGMENT_SHADER, fp);
	//��������Ƭ����ɫ������
	GLuint vfprogram = glCreateProgram();
	//����������
	glAttachShader(vfprogram, vShader);
	glAttachShader(vfprogram, fShader);
	finalizeShaderProgram(vfprogram);

	return vfprogram;
}

//�������㡢���Ρ�Ƭ����ɫ������
GLuint Utils::createShaderProgram(const char *vp, const char *gp, const char * fp)
{
	//Ԥ���붥�㡢���Ρ�Ƭ�β���
	GLuint vShader = prepareShader(GL_VERTEX_SHADER, vp);
	GLuint gShader = prepareShader(GL_GEOMETRY_SHADER, gp);
	GLuint fShader = prepareShader(GL_FRAGMENT_SHADER, fp);
	//��������Ƭ����ɫ������
	GLuint vfprogram = glCreateProgram();
	//����������
	glAttachShader(vfprogram, vShader);
	glAttachShader(vfprogram, gShader);
	glAttachShader(vfprogram, fShader);
	finalizeShaderProgram(vfprogram);

	return vfprogram;
}

//�������㡢����ϸ�֡�Ƭ����ɫ������
GLuint Utils::createShaderProgram(const char* vp, const char* tCS, const char* tES, const char* fp)
{
	GLuint vShader = prepareShader(GL_VERTEX_SHADER, vp);
	GLuint tcShader = prepareShader(GL_TESS_CONTROL_SHADER, tCS);
	GLuint teShader = prepareShader(GL_TESS_EVALUATION_SHADER, tES);
	GLuint fShader = prepareShader(GL_FRAGMENT_SHADER, fp);
	GLuint vtfprogram = glCreateProgram();
	glAttachShader(vtfprogram, vShader);
	glAttachShader(vtfprogram, tcShader);
	glAttachShader(vtfprogram, teShader);
	glAttachShader(vtfprogram, fShader);
	finalizeShaderProgram(vtfprogram);
	return vtfprogram;
}

//�������㡢����ϸ�֡����Ρ��ж���ɫ������
GLuint Utils::createShaderProgram(const char* vp, const char* tCS, const char* tES, const char* gp, const char* fp)
{
	GLuint vShader = prepareShader(GL_VERTEX_SHADER, vp);
	GLuint tcShader = prepareShader(GL_TESS_CONTROL_SHADER, tCS);
	GLuint teShader = prepareShader(GL_TESS_EVALUATION_SHADER, tES);
	GLuint gShader = prepareShader(GL_GEOMETRY_SHADER, gp);
	GLuint fShader = prepareShader(GL_FRAGMENT_SHADER, fp);
	GLuint vtgfprogram = glCreateProgram();
	glAttachShader(vtgfprogram, vShader);
	glAttachShader(vtgfprogram, tcShader);
	glAttachShader(vtgfprogram, teShader);
	glAttachShader(vtgfprogram, gShader);
	glAttachShader(vtgfprogram, fShader);
	finalizeShaderProgram(vtgfprogram);
	return vtgfprogram;
}
