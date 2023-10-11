#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<SOIL2/soil2.h>
#include<iostream>
#include<string>
#include<fstream>	//通过fstream这个文件流来实现的。当我们使用#includ 时，我们就可以使用其中的 ifstream,ofstream以及fstream 这三个类了(ofstream是从内存到硬盘，ifstream是从硬盘到内存)，也就可以用这三个类来定义相应的对象了，这三个类分别代表一个输入文件，一个输出文件，以及一个输入输出文件
#include<cmath>
#include<vector>
#include<glm/glm.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtc/matrix_transform.hpp>

class Utils
{
private:
	static std::string readShaderFile(const char* filePath);				//读取文件
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