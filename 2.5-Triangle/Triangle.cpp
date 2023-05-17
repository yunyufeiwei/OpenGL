using namespace std;

#include<iostream>
#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<fstream>
#include<string>

#define numVAOs 1

GLuint renderingProgram;
GLuint vao[numVAOs];

//��ȡ�ļ�
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
	//�����洢����
	GLuint vfprogram = glCreateProgram();

	string vertShaderStr = readFile("vertShader.glsl");
	string fragShaderStr = readFile("fragShader.glsl");
	const char* vertShaderSrc = vertShaderStr.c_str();
	const char* fragShaderSrc = fragShaderStr.c_str();

	//����glShaderSource()���������ڽ�GLSL������ַ����������ɫ�������У�֮����ʹ��glCompilShader()�����������ɫ��
	//glShaderSource��Ҫ����4��������1-���������ɫ������2-��ɫ��Դ�����е��ַ������� 3-����Դ������ַ���ָ�� 4-��������
	glShaderSource(vShader, 1, &vertShaderSrc, NULL);
	glShaderSource(fShader, 1, &fragShaderSrc, NULL);
	//������ɫ��
	glCompileShader(vShader);
	glCompileShader(fShader);

	//����������
	glAttachShader(vfprogram, vShader);
	glAttachShader(vfprogram, fShader);
	//����GLSL������ȷ��������
	glLinkProgram(vfprogram);

	return vfprogram;
}

void init(GLFWwindow* window)
{
	renderingProgram = createShaderProgram();
	//����OpenGL��Ҫ�Ļ�����
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

	//windowHintָ������������OpenGL�汾����4.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);		//���汾��
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);		//�ΰ汾��

	//���崰�ڵĳ���ߴ磬��һ��������ʾ�˿��ڶ���������ʾ�ߣ�������������ʾ��������
	GLFWwindow* window = glfwCreateWindow(600, 600, "Triangle", NULL, NULL);

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