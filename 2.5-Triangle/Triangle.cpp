using namespace std;

#include<iostream>
#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<fstream>
#include<string>

#define numVAOs 1

GLuint renderingProgram;
GLuint vao[numVAOs];

//读取文件
string readFile(const char* filePath)
{
	string content;
	ifstream filestream(filePath, ios::in);
	string line = "";
	while (!filestream.eof())
	{
		getline(filestream, line);
		content.append(line + "\n");
	}
	filestream.close();
	return content;
}


GLuint createShaderProgram()
{
	GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);
	//创建存储对象
	GLuint vfprogram = glCreateProgram();

	string vertShaderStr = readFile("vertShader.glsl");
	string fragShaderStr = readFile("fragShader.glsl");
	const char* vertShaderSrc = vertShaderStr.c_str();
	const char* fragShaderSrc = fragShaderStr.c_str();

	//调用glShaderSource()函数，用于将GLSL代码从字符串载入空着色器对象中，之后在使用glCompilShader()函数编译各着色器
	//glShaderSource需要传入4个参数，1-用来存放着色器对象，2-着色器源代码中的字符串数量 3-包含源代码的字符串指针 4-长度数组
	glShaderSource(vShader, 1, &vertShaderSrc, NULL);
	glShaderSource(fShader, 1, &fragShaderSrc, NULL);
	//编译着色器
	glCompileShader(vShader);
	glCompileShader(fShader);

	//加入程序对象
	glAttachShader(vfprogram, vShader);
	glAttachShader(vfprogram, fShader);
	//请求GLSL编译器确保兼容性
	glLinkProgram(vfprogram);

	return vfprogram;
}

void init(GLFWwindow* window)
{
	renderingProgram = createShaderProgram();
	//创建OpenGL需要的缓冲区
	glGenVertexArrays(numVAOs, vao);
	glBindVertexArray(vao[0]);
}

void display(GLFWwindow* window, double currentTime)
{
	glUseProgram(renderingProgram);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

void main()
{
	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}

	//windowHint指定机器必须与OpenGL版本兼容4.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);		//主版本号
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);		//次版本号

	//定义窗口的长宽尺寸，第一个参数表示了宽，第二个参数表示高，第三个参数表示窗口名称
	GLFWwindow* window = glfwCreateWindow(600, 600, "Triangle", NULL, NULL);

	//创建GLFW窗口并不会自动将它与当前OpenGL上下文关联，需要调用glfwMakeContextCurrent()
	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK)
	{
		exit(EXIT_FAILURE);
	}
	glfwSwapInterval(1);

	init(window);

	while (!glfwWindowShouldClose(window))
	{
		display(window, glfwGetTime());
		//后置缓缓冲区与前置缓冲区交换
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}