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
	while (!fileStream.eof())			//EOF(filenumber)函数返回False，直到达到文件的末尾。当达到末尾就返回True，执行while函数内的部分
	{
		getline(fileStream, line);
		content.append(line + "\n");
	}
	fileStream.close();
	return content;
}

//检车错误的log函数实现，类型布尔型
bool Utils::checkOpenGLError()
{
	bool foundError = false;		//定义一个布尔变量foundError
	int glErr = glGetError();		//glGetError获取错误的信息，并指定到glErr变量
	while (glErr != GL_NO_ERROR)	//GL_NO_ERROR,未记录任何错误，此符号常量的只保证为零，当glErr不等于，意味着发现了错误
	{
		cout << "glError:" << glErr << endl;		//输出glError的错误log提示
		foundError = true;							//当发现了错误时，修改foundError的值为1
		glErr = glGetError();						//输出获取到的信息并指定
	}
	return foundError;
}

//当GLSL编译失败时，显示OpenGL日志内容
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

//准备Shader
GLuint Utils::prepareShader(int shaderTYPE, const char* shaderPath)
{
	GLint shaderCompiled;
	//定义一个变量shaderStr用来读取文件(顶点/片段着色器)
	string shaderStr = readShaderFile(shaderPath);	
	const char* shaderSrc = shaderStr.c_str();
	//定义一个变量shaderRef来创建着色器
	GLuint shaderRef = glCreateShader(shaderTYPE);		
	//调用glShaderSource()函数，用于将GLSL代码从字符串载入空着色器对象中，之后在使用glCompilShader()函数编译各着色器
	//glShaderSource需要传入4个参数，1-用来存放着色器对象，2-着色器源代码中的字符串数量 3-包含源代码的字符串指针 4-长度数组
	glShaderSource(shaderRef, 1, &shaderSrc, NULL);
	//编译shader
	glCompileShader(shaderRef);
	//编译shader
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

//当GLSL链接着色器失败时，显示OpenGL日志内容
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
		cout << "Linking failed ：" << endl;
		printProgramLog(sprogram);
	}
	return sprogram;
}

//创建顶点片段着色器程序
GLuint Utils::createShaderProgram(const char *vp, const char *fp)
{
	//预编译顶点、片段材质
	GLuint vShader = prepareShader(GL_VERTEX_SHADER, vp);
	GLuint fShader = prepareShader(GL_FRAGMENT_SHADER, fp);
	//创建顶点片段着色器程序
	GLuint vfprogram = glCreateProgram();
	//加入程序对象
	glAttachShader(vfprogram, vShader);
	glAttachShader(vfprogram, fShader);
	finalizeShaderProgram(vfprogram);

	return vfprogram;
}

//创建顶点、几何、片段着色器程序
GLuint Utils::createShaderProgram(const char *vp, const char *gp, const char * fp)
{
	//预编译顶点、几何、片段材质
	GLuint vShader = prepareShader(GL_VERTEX_SHADER, vp);
	GLuint gShader = prepareShader(GL_GEOMETRY_SHADER, gp);
	GLuint fShader = prepareShader(GL_FRAGMENT_SHADER, fp);
	//创建顶点片段着色器程序
	GLuint vfprogram = glCreateProgram();
	//加入程序对象
	glAttachShader(vfprogram, vShader);
	glAttachShader(vfprogram, gShader);
	glAttachShader(vfprogram, fShader);
	finalizeShaderProgram(vfprogram);

	return vfprogram;
}

//创建顶点、曲面细分、片段着色器程序
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

//创建顶点、曲面细分、几何、判断着色器程序
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
