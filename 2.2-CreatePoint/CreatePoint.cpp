using namespace std;

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <iostream>

#define numVAOs 1

//����һ����Ⱦ��������
GLuint renderingProgram;
//��һ��burrfer�Ļ�������OpenGL����Ҫ����һ��
GLuint vao[numVAOs];

//������ɫ������
GLuint createShaderProgram()
{
	//�����ַ���ΪvshaderSource�Ķ�����ɫ��
	const char * vshaderSource = 
		"#version 430    \n"
		"void main(void) \n"
		"{ gl_Position = vec4(0.0, 0.0, 0.0, 1.0); }";

	//�����ַ���ΪfshaderSource��Ƭ����ɫ��
	const char* fshaderSource =
		"#version 430    \n"
		"out vec4 color; \n"						//colorע���������������ɫ
		"void main(void)\n"
		"{ color = vec4(0.0, 1.0, 0.0, 1.0); }";	//����Ƭ�λ��Ƶ���ɫ
		//"void main(void){if (gl_GragCoord.x < 200) color = vec4(1.0, 0.0, 0.0, 1.0);else color = vec4(0.0, 0.0, 1.0, 1.0);}";

	//OpenGL����ÿ����ɫ�����󣨳�ʼֵΪ�գ���ʱ�򣬻᷵��һ������ID��Ϊ��������������ţ����뽫���ID������vShader��fShader������
	GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);
	//�����洢����
	GLuint vfprogram = glCreateProgram();

	//����glShaderSource()���������ڽ�GLSL������ַ����������ɫ�������У�֮����ʹ��glCompilShader()�����������ɫ��
	//glShaderSource��Ҫ����4��������1-���������ɫ������2-��ɫ��Դ�����е��ַ������� 3-����Դ������ַ���ָ�� 4-��������
	glShaderSource(vShader, 1, &vshaderSource, NULL);
	glShaderSource(fShader, 1, &fshaderSource, NULL);
	//������ɫ��
	glCompileShader(vShader);
	glCompileShader(fShader);

	//����ɫ������������
	glAttachShader(vfprogram, vShader);
	glAttachShader(vfprogram, fShader);
	//����GLSL������ȷ��������
	glLinkProgram(vfprogram);

	return vfprogram;
	
}

//���崰�ڵĺ���
void init(GLFWwindow* window)
{
	//����һ������createShaderProgram()�ĺ���---ǰ���Զ����һ������������ֵ��renderingProgram���Զ���һ������
	renderingProgram = createShaderProgram();
	//����OpenGL��Ҫ�Ļ�����
	glGenVertexArrays(numVAOs, vao);
	glBindVertexArray(vao[0]);
}

//���崰����ɫ
void display(GLFWwindow* window, double currentTime)
{
	glUseProgram(renderingProgram);

	//������Ⱦ��Ĵ�С
	glPointSize(20.0f);
	//����ͼԪ����һ����������ģʽ(�㡢�ߡ�������)���ڶ�������ָ���������ʼ����������������ָ��Ҫ���ֵ��������������磺�����3���㣬�Ǿ���3��
	glDrawArrays(GL_POINTS, 0, 1);
}

int main(void)
{

	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}

	//windowHintָ������������OpenGL�汾����4.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//���崰�ڵĳ���ߴ�
	GLFWwindow* window = glfwCreateWindow(600, 600, "Create Point", NULL, NULL);

	//����GLFW���ڲ������Զ������뵱ǰOpenGL�����Ĺ�������Ҫ����glfwMakeContextCurrent()
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
		//���û���������ǰ�û���������
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}