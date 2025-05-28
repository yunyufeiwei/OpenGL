using namespace std;

#include<iostream>
#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<fstream>
#include<string>

#define numVAOs 1

float x = 0.0f;
float inc = 0.01f;

GLuint renderingProgram;
GLuint vao[numVAOs];			//顶点数组对象VAO（Vertex Array Object)

//读取文件,
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

	//当所读取的文件和当前cpp文件同级别时，直接使用"vertShader.glsl"，当在该cpp文件同目录的下个某个文件夹内时，使用"glsl/vertShader.glsl"，如果上一级别，使用"../glsl/vertShader.glsl"
	string vertShaderStr = readFile("glsl/vertShader.glsl");
	string fragShaderStr = readFile("glsl/fragShader.glsl");
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
	//创建OpenGL需要的顶点数组对象缓冲区
	glGenVertexArrays(numVAOs, vao);
	glBindVertexArray(vao[0]);
}

void display(GLFWwindow* window, double currentTime)
{
	glClear(GL_DEPTH_BUFFER_BIT);			//每帧之前清楚深度缓冲,由于这里没有使用到深度，因为注释改行在视觉上看不到太明显的效果
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);			//每次将背景清楚为黑色,如果不执行Buffer清除，会出现类似残留的覆盖效果

	glUseProgram(renderingProgram);

	//移动动画
	x += inc;								//切换至让三角形向右移动
	if (x > 1.0f) inc = -0.01f;				//沿X轴移动三角形
	if (x < -1.0f) inc = 0.01f;				//切换至让三角形向左移动
	GLuint offsetLoc = glGetUniformLocation(renderingProgram, "offset");		//获取offset的指针
	glProgramUniform1f(renderingProgram, offsetLoc, x);

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
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);		//次版本号、

	//定义窗口的长宽尺寸，第一个参数表示了宽，第二个参数表示高，第三个参数表示窗口名称
	GLFWwindow* window = glfwCreateWindow(600, 600, "Animation", NULL, NULL);

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