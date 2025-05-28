using namespace std;

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <iostream>

#define numVAOs 1

//定义一个渲染程序类型
GLuint renderingProgram;
//顶一个burrfer的缓冲区，OpenGL至少要定义一个
GLuint vao[numVAOs];


//定义着色器函数
GLuint createShaderProgram()
{
	//定义字符串为vshaderSource的顶点着色器
	const char* vshaderSource = 
		"#version 430    \n"
		"void main(void) \n"
		"{ gl_Position = vec4(0.5f, 0.0, 0.0, 1.0); }";	//设置点绘制的位置

	//定义字符串为fshaderSource的片段着色器
	const char* fshaderSource =
		"#version 430    \n"
		"out vec4 color; \n"						//color注明这里输出的是颜色
		"void main(void)\n"
		"{ color = vec4(0.0, 1.0, 0.0, 1.0); }";	//定义片段绘制的颜色
		//"void main(void){if (gl_GragCoord.x < 200) color = vec4(1.0, 0.0, 0.0, 1.0);else color = vec4(0.0, 0.0, 1.0, 1.0);}";

	//OpenGL创建每个着色器对象（初始值为空）的时候，会返回一个横竖ID作为后面引用它的序号，代码将这个ID存入了vShader和fShader变量中
	GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);
	//创建存储对象
	GLuint vfprogram = glCreateProgram();

	//调用glShaderSource()函数，用于将GLSL代码从字符串载入空着色器对象中，之后在使用glCompilShader()函数编译各着色器
	//glShaderSource需要传入4个参数，1-用来存放着色器对象，2-着色器源代码中的字符串数量 3-包含源代码的字符串指针 4-长度数组
	glShaderSource(vShader, 1, &vshaderSource, NULL);
	glShaderSource(fShader, 1, &fshaderSource, NULL);
	//编译着色器
	glCompileShader(vShader);
	glCompileShader(fShader);

	//将着色器加入程序对象
	glAttachShader(vfprogram, vShader);
	glAttachShader(vfprogram, fShader);

	//请求GLSL编译器确保兼容性
	glLinkProgram(vfprogram);

	return vfprogram;
	}

//定义窗口的函数
void init(GLFWwindow* window)
{
	//调用一个叫做createShaderProgram()的函数---前面自定义的一个函数，并赋值给renderingProgram，自定的一个类型
	renderingProgram = createShaderProgram();
	//创建OpenGL需要的缓冲区
	glGenVertexArrays(numVAOs, vao);
	glBindVertexArray(vao[0]);
}

//定义窗口颜色
void display(GLFWwindow* window, double currentTime)
{
	glUseProgram(renderingProgram);

	//设置渲染点的大小
	glPointSize(5.0f);

	//绘制图元，第一个参数绘制模式(点、线、三角形)，第二个参数指定数组的起始索引，第三个参数指定要呈现的索引数量（例如：如果有3个点，那就是3）
	glDrawArrays(GL_POINTS, 0, 1);
}

int main(void)
{
	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}

	//windowHint指定机器必须与OpenGL版本兼容4.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//定义窗口的长宽尺寸、窗口名称
	GLFWwindow* window = glfwCreateWindow(600, 600, "Create Point", NULL, NULL);

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
		
		//处理窗口相关事件（比如键盘输入、鼠标移动）、更新窗口状态，并调用对应的回调函数
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	//当渲染循环结束后，需要正确释放/删除之前分配的所有资源，可以在main函数的最后调用glfwTerminate函数来完成。
	glfwTerminate();
	exit(EXIT_SUCCESS);
}
