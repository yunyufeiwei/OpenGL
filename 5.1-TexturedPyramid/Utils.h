#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<SOIL2/soil2.h>
#include<iostream>
#include<string>
#include<fstream>	//ͨ��fstream����ļ�����ʵ�ֵġ�������ʹ��#includ ʱ�����ǾͿ���ʹ�����е� ifstream,ofstream�Լ�fstream ����������(ofstream�Ǵ��ڴ浽Ӳ�̣�ifstream�Ǵ�Ӳ�̵��ڴ�)��Ҳ�Ϳ���������������������Ӧ�Ķ����ˣ���������ֱ����һ�������ļ���һ������ļ����Լ�һ����������ļ�
#include<cmath>
#include<vector>
#include<glm/glm.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtc/matrix_transform.hpp>

class Utils
{
private:
	static std::string readShaderFile(const char* filePath);				//��ȡ�ļ�
	static void printShaderLog(GLuint shader);
	static void printProgramLog(int prog);
	static GLuint prepareShader(int shaderTYPE, const char* shaderPath);
	static int finalizeShaderProgram(GLuint sprogram);

public:
	Utils();
	static bool checkOpenGLError();
	static GLuint createShaderProgram(const char* vp, const char* fp);
	static GLuint createShaderProgram(const char* vp, const char* gp, const char* fp);
	static GLuint createShaderProgram(const char* vp, const char* tCS, const char* tES, const char* fp);
	static GLuint createShaderProgram(const char* vp, const char* tCS, const char* tES, const char* gp, const char* fp);

	static GLuint loadTexture(const char* texImagePath);

};