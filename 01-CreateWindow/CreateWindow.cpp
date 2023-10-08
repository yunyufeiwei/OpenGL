using namespace std;

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <iostream>

//定义窗口的函数
void init(GLFWwindow* window) { }

//定义窗口显示状态
void display(GLFWwindow* window, double currentTime) 
{
	//指定清除背景时使用的颜色，末尾1表示不透明度
	glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
	//实际使用输入颜色对颜色缓冲区进行填充
	glClear(GL_COLOR_BUFFER_BIT);
}

int main(void) 
{
	//glfwInit()函数是一个Int类型的无返回值的
	//如果glfw没有初始化，那么就执行exit中的EXIT_FAILURE,而#define EXIT_FAILURE 1，因此glfwInit()又变成了1
	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}	

	//windowHint指定机器必须与OpenGL版本兼容4.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);		//主版本号
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);		//次版本号

	//glfwCreateWindow()命令负责创建GLFW窗口，参数定义窗口的长宽尺寸，第一个参数表示了宽，第二个参数表示高，第三个参数表示窗口名称,
	GLFWwindow* window = glfwCreateWindow(600, 600, "Create Window", NULL, NULL);	

	//创建GLFW窗口并不会自动将它与当前OpenGL上下文关联，需要调用glfwMakeContextCurrent()
	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK)
	{
		exit(EXIT_FAILURE); 
	}
	glfwSwapInterval(1);

	init(window);


	//渲染循环
	while (!glfwWindowShouldClose(window))
	{
		//执行显示函数
		display(window, glfwGetTime());

		//后置缓缓冲区与前置缓冲区交换
		glfwSwapBuffers(window);

		//处理窗口相关事件（如案件事件）
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}