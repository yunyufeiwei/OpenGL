#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <SOIL2/SOIL2.h>                //引入纹理库
#include <string>
#include <iostream>
#include <fstream>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Utils.h"
#include "Torus.h"          //使用环形模型
using namespace std;

//定义缓冲区
#define numVAOs 1
#define numVBOs 4	

float toRadians(float degrees) { return (degrees * 2.0f * 3.14159f) / 360.0f; }

//声明变量，分配内存
float cameraX, cameraY, cameraZ;
float torLocX, torLocY, torLocZ;
GLuint renderingProgram;
GLuint vao[numVAOs];
GLuint vbo[numVBOs];
GLuint globalAmbLoc, ambLoc, diffLoc, specLoc, posLoc, mambLoc, mdiffLoc, mspecLoc, mshiLoc;	//着色器变量

//声明透视矩阵相关变量
GLuint mvLoc, projLoc, nLoc;		//mv矩阵，透视矩阵
int width, height;
float aspect;
glm::mat4 pMat, vMat, mMat, mvMat, invTrMat,rMat;

Torus myTorus(0.5f, 0.2f, 48);
int numTorusVertices = myTorus.getNumVertices();    //声明模型顶点数据，通过Torus定义来获取顶点数
int numTorusIndices = myTorus.getNumIndices();      //声明模型顶点索引，通过Torus定义来获得索引序号

//声明光照相关数据
float lightPos[3];									//光照位置的浮点数组
glm::vec3 currentLightPos, lightPosV;				//在模型和视觉空间中的光照位置，Vector3f类型
glm::vec3 initialLightLoc = glm::vec3(5.0f, 2.0f, 2.0f);	//初始化光照位置
float amt = 0.0f;
//白光特性
float globalAmbient[4] = { 0.7f, 0.7f, 0.7f, 1.0f };	//环境光
float lightAmbient[4] = { 0.0f, 0.0f, 0.0f, 1.0f };		//白光的环境光强为0
float lightDiffuse[4] = { 1.0f, 1.0f, 1.0f, 1.0f };		//白光的漫反射颜色为白色（1，1,1,1）
float lightSpecular[4] = { 1.0f, 1.0f, 1.0f, 1.0f };	//白光的高光颜色为白色（1,1,1,1）

//黄金材质特性
float* matAmb = Utils::goldAmbient();
float* matDif = Utils::goldDiffuse();
float* matSpe = Utils::goldSpecular();
float  matShi = Utils::goldShininess();

void installLights(glm::mat4 vMatrix)
{
	//通过乘以vMatrix（视图矩阵）将光源位置转换为视图空间坐标，并存入浮点数组
	lightPosV = glm::vec3(vMatrix * glm::vec4(currentLightPos, 1.0f));	
	lightPos[0] = lightPosV.x;
	lightPos[1] = lightPosV.y;
	lightPos[2] = lightPosV.z;

	//在着色器中获取光源位置和属性
	globalAmbLoc = glGetUniformLocation(renderingProgram, "goldAmbient");
	ambLoc = glGetUniformLocation(renderingProgram, "light.ambient");
	diffLoc = glGetUniformLocation(renderingProgram, "light.diffuse");
	specLoc = glGetUniformLocation(renderingProgram, "light.specular");
	posLoc = glGetUniformLocation(renderingProgram, "light.position");
	//在着色器中获取材质的属性
	mambLoc = glGetUniformLocation(renderingProgram, "material.ambient");
	mdiffLoc = glGetUniformLocation(renderingProgram, "material.diffuse");
	mspecLoc = glGetUniformLocation(renderingProgram, "material.specular");
	mshiLoc = glGetUniformLocation(renderingProgram, "material.shininess");

	//在着色器中为光源与材质统一变量赋值
	glProgramUniform4fv(renderingProgram, globalAmbLoc, 1, globalAmbient);
	glProgramUniform4fv(renderingProgram, ambLoc, 1, lightAmbient);
	glProgramUniform4fv(renderingProgram, diffLoc, 1, lightDiffuse);
	glProgramUniform4fv(renderingProgram, specLoc, 1, lightSpecular);
	glProgramUniform3fv(renderingProgram, posLoc, 1, lightPos);
	glProgramUniform4fv(renderingProgram, mambLoc, 1, matAmb);
	glProgramUniform4fv(renderingProgram, mdiffLoc, 1, matDif);
	glProgramUniform4fv(renderingProgram, mspecLoc, 1, matSpe);
	glProgramUniform1f(renderingProgram, mshiLoc, matShi);

}

//顶点数据函数
void setupVertices(void)
{
	std::vector<int> ind = myTorus.getIndices();
	std::vector<glm::vec3> vert = myTorus.getVertices();
	std::vector<glm::vec2> tex = myTorus.getTexCoords();
	std::vector<glm::vec3> norm = myTorus.getNormals();

	std::vector<float> pvalues;
	std::vector<float> tvalues;
	std::vector<float> nvalues;

	for (int i = 0; i < myTorus.getNumVertices(); i++)
	{
		pvalues.push_back((vert[ind[i]]).x);
		pvalues.push_back((vert[ind[i]]).y);
		pvalues.push_back((vert[ind[i]]).z);
		tvalues.push_back((tex[ind[i]]).s);
		tvalues.push_back((tex[ind[i]]).t);
		nvalues.push_back((norm[ind[i]]).x);
		nvalues.push_back((norm[ind[i]]).y);
		nvalues.push_back((norm[ind[i]]).z);
	}

	//VAO顶点数组对象
	glGenVertexArrays(1, vao);
	glBindVertexArray(vao[0]);
	glGenBuffers(numVBOs, vbo);

	//VBO顶点缓冲对象，需要两个VBO来存储顶点缓存与纹理缓存
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, pvalues.size() * 4, &pvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, tvalues.size() * 4, &tvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, nvalues.size() * 4, &nvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[3]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, ind.size() * 4, &ind[0], GL_STATIC_DRAW);
}

//初始化函数
void init(GLFWwindow* window)
{
	//创建渲染程序，调用顶点着色器和片段着色器
	renderingProgram = Utils::createShaderProgram("vertShader.glsl", "fragShader.glsl");

	cameraX = 0.0f; cameraX = 0.0f; cameraZ = 1.0f;
	torLocX = 0.0f; torLocY = 0.0f; torLocZ = -1.0f;
	
	//透视矩阵计算放在初始化阶段，可以优化代码
	glfwGetFramebufferSize(window, &width, &height);
	aspect = (float)width / (float)height;
	pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);			//透视矩阵

	setupVertices();
}

//窗口显示函数
void display(GLFWwindow* window, double currentTime)
{
	//清除深度缓冲区
	glClear(GL_DEPTH_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT);

	//执行渲染管线
	glUseProgram(renderingProgram);

	mvLoc = glGetUniformLocation(renderingProgram, "mv_matrix");			//通过获取顶点着色器中定义的mv_matrix矩阵，赋值给mvLoc变量进行计算
	projLoc = glGetUniformLocation(renderingProgram, "proj_matrix");		//通过获取顶点着色器中定义的proj_matrix矩阵，赋值给proLoc变量进行计算
	nLoc = glGetUniformLocation(renderingProgram, "norm_matrix");

	vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX, -cameraY, -cameraZ));	//计算视图矩阵，通过设置的相机位置来计算视图矩阵

	mMat = glm::translate(glm::mat4(1.0f), glm::vec3(torLocX, torLocY, torLocZ));	//基于换面位置，构建模型矩阵
	mMat *= glm::rotate(mMat, toRadians(35.0f), glm::vec3(1.0f, 0.0f, 0.0f));		//旋转换面以便容易看到

	currentLightPos = glm::vec3(initialLightLoc.x, initialLightLoc.y, initialLightLoc.z);
	//amt += 0.5f;
	//rMat = glm::rotate(glm::mat4(1.0f), toRadians(amt), glm::vec3(0.0f, 0.0f, 1.0f));
	//currentLightPos = glm::vec3(rMat * glm::vec4(currentLightPos, 1.0f));

	installLights(vMat);

	mvMat = vMat * mMat;

	invTrMat = glm::transpose(glm::inverse(mvMat));

	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
	glUniformMatrix4fv(nLoc, 1, GL_FALSE, glm::value_ptr(invTrMat));

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[3]);
	glDrawElements(GL_TRIANGLES, numTorusIndices, GL_UNSIGNED_INT, 0);
}

//窗口回调函数
void window_size_callback(GLFWwindow* win, int newWidth, int newHeight)
{
	//如果窗口有调整，则使用新的窗口宽高计算出比例
	aspect = (float)newWidth / (float)newHeight;
	glViewport(0, 0, newWidth, newHeight);
	pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);
}

//主函数，运行
int main()
{
	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	GLFWwindow* window = glfwCreateWindow(600, 600, "ProgramModel", NULL, NULL);
	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK)
	{
		exit(EXIT_FAILURE);
	}

	glfwSwapInterval(1);

	//初始化窗口时，执行一下回调函数，修改窗口的改变产生的渲染问题
	glfwSetWindowSizeCallback(window, window_size_callback);

	//执行初始化函数，需要传window的参数进来
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

