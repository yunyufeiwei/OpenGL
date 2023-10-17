#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <SOIL2/SOIL2.h>                //引入纹理库
#include <string>
#include <iostream>
#include <fstream>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Utils.h"
#include "Torus.h"
#include "ImportedModel.h"
using namespace std;

#define numVAOs 1   //声明一个顶点数组对象,VAO(Vertex Array Object)顶点数组对象
#define numVBOs 5   //声明5个顶点缓冲对象,VBO(Vertex Buffer Object)顶点缓冲对象

//声明渲染程序变量
GLuint renderingProgram1, renderingProgram2;
GLuint vao[numVAOs];
GLuint vbo[numVBOs];

//金字塔属性
ImportedModel pyramid("pyr.obj");   //导入金字塔模型
int numPyramidVertices;             //声明金字塔的顶点数量

//环形模型属性
Torus myTorus(0.6f, 0.4f, 48);      //声明环形模型大小
int numTorusVertices;               //声明环形模型顶点数     
int numTorusIndices;                //声明环形模型顶点索引数

//显示变量分配
int width, height;                  //声明窗口的宽和高
float aspect;                       //声明宽高比
glm::mat4 pMat;
glm::vec3 currentLightPos, transformed;
float lightPos[3];
GLuint globalAmbLoc , ambLoc, diffLoc, specLoc, posLoc, mambLoc, mdiffLoc, mspecLoc, mshiLoc;

//白光
float globalAmbient[4] = { 0.7f, 0.7f, 0.7f, 1.0f };
float lightAmbient[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
float lightDiffuse[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
float lightSpecular[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

//黄金材质
float* gMatAmb = Utils::goldAmbient();
float* gMatDif = Utils::goldDiffuse();
float* gMatSpe = Utils::goldSpecular();
float gMatShi = Utils::goldShininess();

void installLights(int renderingProgram, glm::mat4 vMatrix)
{

}

void setupVertices(void)
{
    //定义金字塔
    numPyramidVertices = pyramid.getNumVertices();
    std::vector<glm::vec3> vert = pyramid.getVertices();
    std::vector<glm::vec3> norm = pyramid.getNormals();

    std::vector<float> pyramidPvalues;
    std::vector<float> pyramidNvalues;
    for (int i = 0; i < numPyramidVertices; i++) 
    {
        pyramidPvalues.push_back((vert[i]).x);
        pyramidPvalues.push_back((vert[i]).y);
        pyramidPvalues.push_back((vert[i]).z);
        pyramidNvalues.push_back((norm[i]).x);
        pyramidNvalues.push_back((norm[i]).y);
        pyramidNvalues.push_back((norm[i]).z);
    }

    //定义环形模型
    numTorusVertices = myTorus.getNumVertices();
    numTorusIndices = myTorus.getNumIndices();
    std::vector<int> ind = myTorus.getIndices();
    vert = myTorus.getVertices();
    norm = myTorus.getNormals();

    std::vector<float> torusPvalues;
    std::vector<float> torusNvalues;

    for (int i = 0; i < numTorusVertices; i++) 
    {
        torusPvalues.push_back(vert[i].x);
        torusPvalues.push_back(vert[i].y);
        torusPvalues.push_back(vert[i].z);
        torusNvalues.push_back(norm[i].x);
        torusNvalues.push_back(norm[i].y);
        torusNvalues.push_back(norm[i].z);
    }

    glGenVertexArrays(1, vao);  //创建顶点缓冲对象
    glBindVertexArray(vao[0]);  //绑定顶点数组对象
    glGenBuffers(numVBOs, vbo); //创建缓冲区对象

    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);                                                          //激活缓冲区对象，使用vbo第一个缓冲区
    glBufferData(GL_ARRAY_BUFFER, torusPvalues.size() * 4, &torusPvalues[0], GL_STATIC_DRAW);       //用数据分配和初始化缓冲区对象，分配目标是环形模型顶点数
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);                                                          //激活缓冲区对象，使用vbo第二个缓冲区
    glBufferData(GL_ARRAY_BUFFER, pyramidPvalues.size() * 4, &pyramidPvalues[0], GL_STATIC_DRAW);   //用数据分配和初始化缓冲区对象，分配目标是金字塔模型顶点数

    glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
    glBufferData(GL_ARRAY_BUFFER, torusNvalues.size() * 4, &torusNvalues[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);
    glBufferData(GL_ARRAY_BUFFER, pyramidNvalues.size() * 4, &pyramidNvalues[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[4]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, ind.size() * 4, &ind[0], GL_STATIC_DRAW);
}

//初始化窗口
void init(GLFWwindow* window)
{
    //声明渲染程序，调用了封装在Utils文件里面的方法
    renderingProgram1 = Utils::createShaderProgram("vert1Shader.glsl", "frag1Shader.glsl");
    renderingProgram2 = Utils::createShaderProgram("vert2Shader.glsl", "frag2Shader.glsl");

    glfwGetFramebufferSize(window, &width, &height);
    aspect = (float)width / (float)height;
    pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);

    setupVertices();
}

void display(GLFWwindow* window, double currentTime)
{
    glClear(GL_DEPTH_BUFFER_BIT);
    glClear(GL_COLOR_BUFFER_BIT);

    
}

int main()
{
    std::cout << "Hello World!\n";
}