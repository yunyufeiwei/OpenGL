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

//定义顶点数组对象
#define numVAOs 1
//定义Buffer缓冲对象
#define numVBOs 2

Utils util = Utils();
float cameraX, cameraY, cameraZ;
float cubeLocX, cubeLocY, cubeLocZ;

GLuint renderingProgram;
GLuint vao[numVAOs];
GLuint vbo[numVBOs];

//分配在display()函数中使用的变量空间，这样他们就不必再渲染过程中分配
GLuint mvLoc, projLoc;
int width, height;
float aspect;
glm::mat4 pMat, vMat, mMat , mvMat;  //定义 v矩阵（视图矩阵),p矩阵（投影矩阵）,mv矩阵（模型-视图矩阵）

//声明一个名为setupVertices的数组，包含36个组成立方体的顶点
void setupVertices(void)
{
	//每3个顶点构成一个三角形，一个正方形有12个三角形，因此总顶点数有3*3*12 = 108，基于三角面的构成与序号和顺序有关，因此其中部分的位置是重复
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

	//创建VAO，绑定VAO，这样生成的缓冲区就会和这个VAO相关联
	glGenVertexArrays(1, vao);
	glBindVertexArray(vao[0]);
	//创建VBO、绑定VBO
	glGenBuffers(numVBOs, vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	//将包含顶点数据的数组赋值进活跃缓冲区（这里应该是第0个VBO）
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);
};

void init(GLFWwindow* window)
{
	//渲染程序，调用了Utils里面封装好的顶点、片段着色器
	renderingProgram = Utils::createShaderProgram("vertShader.glsl" , "fragShader.glsl"); 

	cameraX = 0.0f; cameraY = 0.0f; cameraZ = 8.0f;
	cubeLocX = 0.0f, cubeLocY = -2.0f, cubeLocZ = 0.0f;
	setupVertices();
}

void display(GLFWwindow* window , double currentTime)
{
	//每帧之前清除深度缓冲区
	glClear(GL_DEPTH_BUFFER_BIT);

	//使用glUseProgram()函数来启用着色器
	glUseProgram(renderingProgram);

	//获取MV矩阵和投影矩阵的统一变量
	mvLoc = glGetUniformLocation(renderingProgram, "mv_matrix");
	projLoc = glGetUniformLocation(renderingProgram, "proj_matrix");

	//构建透视矩阵
	glfwGetFramebufferSize(window, &width, &height);
	aspect = (float)width / (float)height;
	pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);   //1.0472 radians = 60 degrees  glm::perspective(<filed of view> , <aspect ratio> , <near plane>)

	//构建视图矩阵、模型矩阵和视图-模型矩阵
	vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX, -cameraY, -cameraZ));
	mMat = glm::translate(glm::mat4(1.0f), glm::vec3(cubeLocX, cubeLocY, cubeLocZ));
	mvMat = vMat * mMat;

	//将透视矩阵和MV矩阵复制给相应的统一变量
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));

	//将VBO关联给顶点着色器中相应的顶点属性
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glVertexAttribPointer(0, 3 , GL_FLOAT , GL_FALSE, 0 , 0);
	glEnableVertexAttribArray(0);

	//调整OpenGL设置，绘制模型
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	//Display()函数做得最后一件事就是通过调用glDrawArrays()来绘制模型，指定模型的组成方式(三角形),并设置顶点数。
	glDrawArrays(GL_TRIANGLES, 0, 36);				

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

	//执行初始化函数，需要传window的参数进来
	init(window);

	//渲染循环
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
