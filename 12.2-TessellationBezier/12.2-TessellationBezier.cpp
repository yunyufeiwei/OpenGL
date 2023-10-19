#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <SOIL2\soil2.h>
#include <string>
#include <iostream>
#include <fstream>
#include <glm\gtc\type_ptr.hpp> // glm::value_ptr
#include <glm\gtc\matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include "Utils.h"
using namespace std;

float toRadians(float degrees) { return (degrees * 2.0f * 3.14159f) / 360.0f; }

#define numVAOs 1       //模拟细分，值需要顶点缓冲区即可

float cameraX, cameraY, cameraZ;        //声明相机的位置属性
float terLocX, terLocY, terLocZ;         //声明地形的位置属性
GLuint renderingProgram;                //声明渲染程序属性
GLuint vao[numVAOs];
GLuint floorTexture;

//分配变量，减少动态分配对内存的消耗
GLuint mvpLoc;
int width, height;                           //声明窗口的宽高
float aspect;
glm::mat4 pMat, vMat, mMat, mvpMat;

float tessInner = 30.0f;
float tessOuter = 20.0f;

void init(GLFWwindow* window)
{
	//初始化渲染程序
	renderingProgram = Utils::createShaderProgram("vertShader.glsl", "tessCShader.glsl", "tessEShader.glsl", "fragShader.glsl");

	//初始化对象位置
	cameraX = 0.0f; cameraY = 0.0f; cameraZ = 4.0f;
	terLocX = 0.0f; terLocY = 0.0f; terLocZ = 0.0f;

	//初始化窗口宽高，以及视图矩阵
	glfwGetFramebufferSize(window, &width, &height);
	aspect = (float)width / (float)height;
	pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);

	floorTexture = Utils::loadTexture("floor_color.jpg");

	//绑定缓冲区
	glGenVertexArrays(numVAOs, vao);
	glBindVertexArray(vao[0]);
}

void display(GLFWwindow* window, double currentTime)
{
	//清楚缓冲区
	glClear(GL_DEPTH_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(renderingProgram);

	vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX, -cameraY, -cameraZ));

	mMat = glm::translate(glm::mat4(1.0f), glm::vec3(terLocX, terLocY, terLocZ));
	mMat = glm::rotate(mMat, toRadians(35.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	mMat = glm::rotate(mMat, toRadians(100.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	mvpMat = pMat * vMat * mMat;

	mvpLoc = glGetUniformLocation(renderingProgram, "mvp_matrix");

	glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(mvpMat));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, floorTexture);

	glFrontFace(GL_CCW);

	glPatchParameteri(GL_PATCH_VERTICES, 16);		
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);		//指定如何光栅化，默认是GL_FILL，而当前使用GL_LINE，就可以看到线框显示方式
	glDrawArrays(GL_PATCHES, 0, 16);

}

void window_size_callback(GLFWwindow* win, int newWidth, int newHeight)
{
	aspect = (float)newWidth / (float)newHeight;
	glViewport(0, 0, newWidth, newHeight);
	pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);
}

int main()
{
	if (!glfwInit()) { exit(EXIT_FAILURE); }
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	GLFWwindow* window = glfwCreateWindow(800, 800, "TessellationBezier", NULL, NULL);
	glfwMakeContextCurrent(window);
	if (glewInit() != GLEW_OK) { exit(EXIT_FAILURE); }
	glfwSwapInterval(1);

	glfwSetWindowSizeCallback(window, window_size_callback);

	init(window);

	while (!glfwWindowShouldClose(window)) {
		display(window, glfwGetTime());
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}

