using namespace std;
#include <iostream>
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <string>
#include <fstream>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Utils.h"

#define numVAOs 1
#define numVBOs 2

Utils util = Utils();
float cameraX, cameraY, cameraZ;
float cubeLocX, cubeLocY, cubeLocZ;

GLuint renderingProgram;
GLuint vao[numVAOs];
GLuint vbo[numVBOs];

//������display()������ʹ�õı����ռ䣬�������ǾͲ�������Ⱦ�����з���
GLuint mvLoc, projLoc;
int width, height;
float aspect;
glm::mat4 pMat, vMat, tMat , rMat , mMat, mvMat;  //���� v������ͼ����),p����ͶӰ����,mv����ģ��-��ͼ����

//����һ����ΪsetupVertices�����飬����36�����������Ķ���
void setupVertices(void)
{
	//ÿ3�����㹹��һ�������Σ�һ����������12�������Σ�����ܶ�������3*3*12 = 108������������Ĺ�������ź�˳���йأ�������в��ֵ�λ�����ظ�
	float vertexPositions[108] =
	{
		-1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f, 1.0f,  1.0f, -1.0f, -1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f, 1.0f, -1.0f,  1.0f, 1.0f,  1.0f, -1.0f,
		1.0f, -1.0f,  1.0f, 1.0f,  1.0f,  1.0f, 1.0f,  1.0f, -1.0f,
		1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f, 1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f, -1.0f,  1.0f,  1.0f, 1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f, 1.0f,  1.0f, -1.0f, 1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f, -1.0f
	};

	glGenVertexArrays(1, vao);
	glBindVertexArray(vao[0]);
	glGenBuffers(numVBOs, vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);
};

void init(GLFWwindow* window)
{
	renderingProgram = Utils::createShaderProgram("vertShader.glsl", "fragShader.glsl");

	//����͸�Ӿ���
	glfwGetFramebufferSize(window, &width, &height);
	aspect = (float)width / (float)height;
	pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);   //1.0472 radians = 60 degrees  glm::perspective(<filed of view> , <aspect ratio> , <near plane>)

	cameraX = 0.0f; cameraY = 0.0f; cameraZ = 8.0f;  //���������λ�ã�cameraZ��ʾ�˿���cube�ľ���
	setupVertices();
}

void display(GLFWwindow* window, double currentTime)
{
	////ÿ֮֡ǰ�����Ȼ�����
	glClear(GL_DEPTH_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(renderingProgram);

	//��ȡMV�����ͶӰ�����ͳһ����
	mvLoc = glGetUniformLocation(renderingProgram, "mv_matrix");
	projLoc = glGetUniformLocation(renderingProgram, "proj_matrix");	

	//ʹ�õ�ǰʱ��������x y z�Ĳ�ͬ�任
	vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX, -cameraY, -cameraZ));

	tMat = glm::translate(glm::mat4(1.0f), glm::vec3(sin(0.35f * currentTime) * 2.0f, cos(0.52f * currentTime) * 2.0f, sin(0.7f * currentTime) * 2.0f));

	//ʹ��1.75��������ת�ٶ�
	rMat = glm::rotate(glm::mat4(1.0f), 1.75f * (float)currentTime, glm::vec3(0.0f, 1.0f, 0.0f));
	rMat = glm::rotate(rMat, 1.75f * (float)currentTime, glm::vec3(1.0f, 0.0f, 0.0f));
	rMat = glm::rotate(rMat, 1.75f * (float)currentTime, glm::vec3(0.0f, 0.0f, 1.0f));
	
	mMat = tMat * rMat;
	mvMat = vMat * mMat;

	//��͸�Ӿ����MV�����Ƹ���Ӧ��ͳһ����
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));

	//��VBO������������ɫ������Ӧ�Ķ�������
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	//����OpenGL���ã�����ģ��
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glDrawArrays(GL_TRIANGLES, 0, 36);				//Display()�����������һ���¾���ͨ������glDrawArrays()������ģ�ͣ�ָ��ģ�͵���ɷ�ʽ(������),�����ö�������

}

int main(void)
{
	if (!glfwInit()) { exit(EXIT_FAILURE); }

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	GLFWwindow* window = glfwCreateWindow(600, 600, "PlainRedCube", NULL, NULL);

	glfwMakeContextCurrent(window);
	if (glewInit() != GLEW_OK)
	{
		exit(EXIT_FAILURE);
	}

	glfwSwapInterval(1);

	//ִ�г�ʼ����������Ҫ��window�Ĳ�������
	init(window);

	while (!glfwWindowShouldClose(window))
	{
		display(window, glfwGetTime());
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}
