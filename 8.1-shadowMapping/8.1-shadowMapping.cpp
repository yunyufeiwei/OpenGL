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

//回调函数
void passOne(void);
void passTwo(void);
float toRadians(float degrees) { return (degrees * 2.0f * 3.14159f) / 360.0f; }

#define numVAOs 1   //声明一个顶点数组对象,VAO(Vertex Array Object)顶点数组对象
#define numVBOs 5   //声明5个顶点缓冲对象,VBO(Vertex Buffer Object)顶点缓冲对象

//声明渲染程序变量
GLuint renderingProgram1, renderingProgram2;
GLuint vao[numVAOs];
GLuint vbo[numVBOs];

//模型属性
ImportedModel pyramid("pyr.obj");   //导入金字塔模型
Torus myTorus(0.6f, 0.4f, 48);      //声明环形模型大小
int numPyramidVertices;             //声明金字塔的顶点数量
int numTorusVertices;               //声明环形模型顶点数     
int numTorusIndices;                //声明环形模型顶点索引数

//声明模型物体位置属性
glm::vec3 torusLocation(1.6f, 0.0f, -0.3f);         //圆环模型的位置
glm::vec3 pyrLocation(-1.0f, 0.1f, 0.3f);                //金字塔模型的位置
glm::vec3 cameraLocation(0.0f, 0.2f, 6.0f);              //摄像机的位置
glm::vec3 lightLocation(-3.8f, 2.2f, 1.1f);              //方向光位置

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

//青铜材质
float* bMatAmb = Utils::bronzeAmbient();
float* bMatDif = Utils::bronzeDiffuse();
float* bMatSpe = Utils::bronzeSpecular();
float bMatShi = Utils::bronzeShininess();

//在display()中将光照传入着色器变量
float thisAmb[4], thisDif[4], thisSpe[4], matAmb[4], matDif[4], matSpe[4];
float thisShi, matShi;

//阴影相关变量
int screenSizeX, screenSizeY;
GLuint shadowTex, shadowBuffer;
glm::mat4 lightVmatrix;
glm::mat4 lightPmatrix;
glm::mat4 shadowMVP1;
glm::mat4 shadowMVP2;
glm::mat4 b;                //纹理坐标变换矩阵

//显示变量分配,提前分配变量，减少动态分配对内存的消耗
GLuint mvLoc, projLoc, nLoc, sLoc;
int width, height;                  //声明窗口的宽和高
float aspect;                       //声明宽高比
glm::mat4 pMat , vMat, mMat, mvMat, invTrMat;
glm::vec3 currentLightPos, transformed;     //
float lightPos[3];
GLuint globalAmbLoc , ambLoc, diffLoc, specLoc, posLoc, mambLoc, mdiffLoc, mspecLoc, mshiLoc;
glm::vec3 origin(0.0f, 0.0f, 0.0f);
glm::vec3 up(0.0f, 1.0f, 0.0f);

void installLights(int renderingProgram, glm::mat4 vMatrix)
{
    transformed = glm::vec3(vMatrix * glm::vec4(currentLightPos, 1.0));
    lightPos[0] = transformed.x;
    lightPos[1] = transformed.y;
    lightPos[2] = transformed.z;

    matAmb[0] = thisAmb[0]; matAmb[1] = thisAmb[1]; matAmb[2] = thisAmb[2]; matAmb[3] = thisAmb[3];
    matDif[0] = thisDif[0]; matDif[1] = thisDif[1]; matDif[2] = thisDif[2]; matDif[3] = thisDif[3];
    matSpe[0] = thisSpe[0]; matSpe[1] = thisSpe[1]; matSpe[2] = thisSpe[2]; matSpe[3] = thisSpe[3];
    matShi = thisShi;

    // get the locations of the light and material fields in the shader
    globalAmbLoc = glGetUniformLocation(renderingProgram, "globalAmbient");
    ambLoc = glGetUniformLocation(renderingProgram, "light.ambient");
    diffLoc = glGetUniformLocation(renderingProgram, "light.diffuse");
    specLoc = glGetUniformLocation(renderingProgram, "light.specular");
    posLoc = glGetUniformLocation(renderingProgram, "light.position");
    mambLoc = glGetUniformLocation(renderingProgram, "material.ambient");
    mdiffLoc = glGetUniformLocation(renderingProgram, "material.diffuse");
    mspecLoc = glGetUniformLocation(renderingProgram, "material.specular");
    mshiLoc = glGetUniformLocation(renderingProgram, "material.shininess");

    //  set the uniform light and material values in the shader
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

//阴影缓冲区
void setupShadowBuffers(GLFWwindow* window)
{
    glfwGetFramebufferSize(window, &width, &height);
    screenSizeX = width;
    screenSizeY = height;

    glGenFramebuffers(1, &shadowBuffer);           //创建自定义的帧缓冲

    glGenTextures(1, &shadowTex);                  //创建阴影纹理
    glBindTexture(GL_TEXTURE_2D, shadowTex);       //激活缓冲区，并存储阴影纹理
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, screenSizeX, screenSizeY, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
    //多级渐进纹理
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
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

    setupShadowBuffers(window);
    b = glm::mat4(0.5f, 0.0f, 0.0f, 0.0f,
                  0.0f, 0.5f, 0.0f, 0.0f,
                  0.0f, 0.0f, 0.5f, 0.0f,
                  0.5f, 0.5f, 0.5f, 1.0f);
}

void display(GLFWwindow* window, double currentTime)
{
    glClear(GL_DEPTH_BUFFER_BIT);
    glClear(GL_COLOR_BUFFER_BIT);

    currentLightPos = glm::vec3(lightLocation);

    lightVmatrix = glm::lookAt(currentLightPos, origin, up);
    lightPmatrix = glm::perspective(toRadians(60.0f), aspect, 0.1f, 1000.0f);

    glBindFramebuffer(GL_FRAMEBUFFER, shadowBuffer);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, shadowTex, 0);
    
    glDrawBuffer(GL_NONE);
    glEnable(GL_DEPTH_TEST);
    //减少阴影伪影（阴影痤疮）
    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(5.0f, 8.0f);

    passOne();

    glDisable(GL_POLYGON_OFFSET_FILL);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, shadowTex);

    glDrawBuffer(GL_FRONT);

    passTwo();
}

void passOne(void)
{
    glUseProgram(renderingProgram1);        //renderingProgram1包含了第一轮中的顶点着色器和片段着色器

    //下面代码段通过从光源角度渲染（环面）获得深度缓冲区
    mMat = glm::translate(glm::mat4(1.0), torusLocation);
    mMat = glm::rotate(mMat, toRadians(25.0f), glm::vec3(1.0f, 0.0f, 0.0f));        //轻微绕X轴旋转方便查看

    //从光源角度绘制，因此使用光源的P,V矩阵
    shadowMVP1 = lightPmatrix * lightVmatrix * mMat;
    sLoc = glGetUniformLocation(renderingProgram1, "shadowMVP1");
    glUniformMatrix4fv(sLoc, 1, GL_FALSE, glm::value_ptr(shadowMVP1));

    //在第一轮中值需要环面的顶点缓冲区，而不需要它的纹理或法向量
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);                      //在setupVertices()函数中，绑定的环行的顶点缓冲区vbo[0]
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glClear(GL_DEPTH_BUFFER_BIT);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[4]);
    glDrawElements(GL_TRIANGLES, numTorusIndices, GL_UNSIGNED_INT, 0);

    //下面代码段通过从光源角度渲染（金字塔）获得深度缓冲区
    mMat = glm::translate(glm::mat4(1.0f), pyrLocation);
    mMat = glm::rotate(mMat, toRadians(30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    mMat = glm::rotate(mMat, toRadians(40.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    shadowMVP1 = lightPmatrix * lightVmatrix * mMat;
    glUniformMatrix4fv(sLoc, 1, GL_FALSE, glm::value_ptr(shadowMVP1));

    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glDrawArrays(GL_TRIANGLES, 0, numPyramidVertices);

}

void passTwo(void)
{
    glUseProgram(renderingProgram2);

    mvLoc = glGetUniformLocation(renderingProgram2, "mv_matrix");
    projLoc = glGetUniformLocation(renderingProgram2, "proj_matrix");
    nLoc = glGetUniformLocation(renderingProgram2, "norm_matrix");
    sLoc = glGetUniformLocation(renderingProgram2, "shadowMVP");

    //绘制环形
    thisAmb[0] = bMatAmb[0]; thisAmb[1] = bMatAmb[1]; thisAmb[2] = bMatAmb[2];  // bronze
    thisDif[0] = bMatDif[0]; thisDif[1] = bMatDif[1]; thisDif[2] = bMatDif[2];
    thisSpe[0] = bMatSpe[0]; thisSpe[1] = bMatSpe[1]; thisSpe[2] = bMatSpe[2];
    thisShi = bMatShi;

    vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraLocation.x, -cameraLocation.y, -cameraLocation.z));

    mMat = glm::translate(glm::mat4(1.0f), torusLocation);
    mMat = glm::rotate(mMat, toRadians(25.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    currentLightPos = glm::vec3(lightLocation);
    installLights(renderingProgram2, vMat);

    mvMat = vMat * mMat;
    invTrMat = glm::transpose(glm::inverse(mvMat));
    shadowMVP2 = b * lightPmatrix * lightVmatrix * mMat;

    glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
    glUniformMatrix4fv(nLoc, 1, GL_FALSE, glm::value_ptr(invTrMat));
    glUniformMatrix4fv(sLoc, 1, GL_FALSE, glm::value_ptr(shadowMVP2));

    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    glClear(GL_DEPTH_BUFFER_BIT);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[4]);
    glDrawElements(GL_TRIANGLES, numTorusIndices, GL_UNSIGNED_INT, 0);

    //绘制金字塔
    thisAmb[0] = gMatAmb[0]; thisAmb[1] = gMatAmb[1]; thisAmb[2] = gMatAmb[2];  // gold
    thisDif[0] = gMatDif[0]; thisDif[1] = gMatDif[1]; thisDif[2] = gMatDif[2];
    thisSpe[0] = gMatSpe[0]; thisSpe[1] = gMatSpe[1]; thisSpe[2] = gMatSpe[2];
    thisShi = gMatShi;

    mMat = glm::translate(glm::mat4(1.0f), pyrLocation);
    mMat = glm::rotate(mMat, toRadians(30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    mMat = glm::rotate(mMat, toRadians(40.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    currentLightPos = glm::vec3(lightLocation);
    installLights(renderingProgram2, vMat);

    mvMat = vMat * mMat;
    invTrMat = glm::transpose(glm::inverse(mvMat));
    shadowMVP2 = b * lightPmatrix * lightVmatrix * mMat;

    glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
    glUniformMatrix4fv(nLoc, 1, GL_FALSE, glm::value_ptr(invTrMat));
    glUniformMatrix4fv(sLoc, 1, GL_FALSE, glm::value_ptr(shadowMVP2));

    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glDrawArrays(GL_TRIANGLES, 0, numPyramidVertices);
}

void window_size_callback(GLFWwindow* win, int newWidth, int newHeight)
{
    aspect = (float)newWidth / (float)newHeight;
    glViewport(0, 0, newWidth, newHeight);
    pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);
}

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