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

Utils::Utils() {}

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
	
	string shaderStr = readShaderFile(shaderPath);		//����һ������shaderStr������ȡ�ļ�(����/Ƭ����ɫ��)
	const char* shaderSrc = shaderStr.c_str();
	GLuint shaderRef = glCreateShader(shaderTYPE);		//����һ������shaderRef��������ɫ��
	
	//����glShaderSource()���������ڽ�GLSL������ַ����������ɫ�������У�֮����ʹ��glCompilShader()�����������ɫ��
	glShaderSource(shaderRef, 1, &shaderSrc, NULL);		//glShaderSource��Ҫ����4��������1-���������ɫ������2-��ɫ��Դ�����е��ַ������� 3-����Դ������ַ���ָ�� 4-��������
	glCompileShader(shaderRef);							//����shader
	checkOpenGLError();									//�쳵����shader������ʾ

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

//��GLSL����ʧ��ʱ����ʾOpenGL��־����
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
GLuint Utils::createShaderProgram(const char* vp, const char* fp)
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
GLuint Utils::createShaderProgram(const char* vp, const char* gp, const char* fp)
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

GLuint Utils::loadTexture(const char* texImagePath)
{
	GLuint textureRef;
	textureRef = SOIL_load_OGL_texture(texImagePath, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
	if (textureRef == 0) cout << "Didnt find texture file" << texImagePath << endl;

	//������ù��ʽ�Զ����
	glBindTexture(GL_TEXTURE_2D, textureRef);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);

	//������ø������Թ���
	if (glewIsSupported("GL_EXT_texture_filter_anisotropic"))				//glewIsSupported()���ò����Կ��Ƿ�֧��AF(�������Թ���)
	{
		GLfloat anisoset = 0.0f;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &anisoset);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisoset);
	}

	return textureRef;
}

//�ƽ����---�����⡢�����䡢���淴��͹����
float* Utils::goldAmbient()   { static float a[4] = { 0.2473f, 0.1995f, 0.0745f, 1 }; return (float*)a; }
float* Utils::goldDiffuse()   { static float a[4] = { 0.7516f, 0.6065f, 0.2265f, 1 }; return (float*)a; }
float* Utils::goldSpecular()  { static float a[4] = { 0.6283f, 0.5559f, 0.3661f, 1 }; return (float*)a; }
float  Utils::goldShininess() { return 51.2f; }

//��������---�����⡢�����䡢���淴��͹����
float* Utils::silverAmbient()  { static float a[4] = { 0.1923f,0.1923f,0.1923f,1 }; return (float*)a; }
float* Utils::silverDiffuse()  { static float a[4] = { 0.5075f, 0.5075f, 0.5075f, 1 }; return (float*)a; }
float* Utils::silverSpecular() { static float a[4] = { 0.5083f, 0.5083f, 0.5083f, 1 }; return (float*)a; }
float Utils::silverShininess() { return 51.2f; }

// ��ͭ����---�����⡢�����䡢���淴��͹���
float* Utils::bronzeAmbient()  { static float a[4] = { 0.2125f, 0.1275f, 0.0540f, 1 }; return (float*)a; }
float* Utils::bronzeDiffuse()  { static float a[4] = { 0.7140f, 0.4284f, 0.1814f, 1 }; return (float*)a; }
float* Utils::bronzeSpecular() { static float a[4] = { 0.3936f, 0.2719f, 0.1667f, 1 }; return (float*)a; }
float Utils::bronzeShininess() { return 25.6f; }