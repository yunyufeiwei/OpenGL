using namespace std;

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <iostream>

//���崰�ڵĺ���
void init(GLFWwindow* window) { }

//���崰����ʾ״̬
void display(GLFWwindow* window, double currentTime) 
{
	//ָ���������ʱʹ�õ���ɫ��ĩβ1��ʾ��͸����
	glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
	//ʵ��ʹ��������ɫ����ɫ�������������
	glClear(GL_COLOR_BUFFER_BIT);
}

int main(void) 
{
	//glfwInit()������һ��Int���͵��޷���ֵ��
	//���glfwû�г�ʼ������ô��ִ��exit�е�EXIT_FAILURE,��#define EXIT_FAILURE 1�����glfwInit()�ֱ����1
	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}	

	//windowHintָ������������OpenGL�汾����4.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);		//���汾��
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);		//�ΰ汾��

	//glfwCreateWindow()����𴴽�GLFW���ڣ��������崰�ڵĳ���ߴ磬��һ��������ʾ�˿��ڶ���������ʾ�ߣ�������������ʾ��������,
	GLFWwindow* window = glfwCreateWindow(600, 600, "Create Window", NULL, NULL);	

	//����GLFW���ڲ������Զ������뵱ǰOpenGL�����Ĺ�������Ҫ����glfwMakeContextCurrent()
	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK)
	{
		exit(EXIT_FAILURE); 
	}
	glfwSwapInterval(1);

	init(window);


	//��Ⱦѭ��
	while (!glfwWindowShouldClose(window))
	{
		//ִ����ʾ����
		display(window, glfwGetTime());

		//���û���������ǰ�û���������
		glfwSwapBuffers(window);

		//����������¼����簸���¼���
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}