#include "glew/glew.h"
#include "glfw/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Utils.h"
#include "camera.h"
#include <iostream>
#include <string>
#include <fstream>

using namespace std;

static const float pai = 3.1415926f;
float toRadians(float degrees) { return degrees * 2.f * pai / (float)360.f; }

static const int screenWidth = 1920;
static const int screenHeight = 1080;

static const int numVAOs = 1;
static const int numVBOs = 4;

GLuint renderingProgram = 0;

GLuint vao[numVAOs] = { 0 };
GLuint vbo[numVBOs] = { 0 };

float cameraX = 0.f, cameraY = 0.f, cameraZ = 0.f;
float terLocX = 0.f, terLocY = 0.f, terLocZ = 0.f;

GLuint mvpLoc = 0;

int width = 0, height = 0;

float aspect = 0.f;
glm::mat4 mMat(1.f), vMat(1.f), pMat(1.f), mvMat(1.f), mvpMat(1.f);
float tessInner = 30.f;
float tessOuter = 20.f;
GLuint floorTexture = 0;


Camera camera(glm::vec3(0.f, 0.f, 5.f));
//float cameraX = 0.f, cameraY = 0.f, cameraZ = 5.f;
GLboolean keys[1024] = { GL_FALSE };
GLboolean b_firstMouse = GL_TRUE;
float deltaTime = 0.f;

float lastFrame = 0.f;
float lastLocX = 0.f;
float lastLocY = 0.f;

void do_movement()
{
	if (keys[GLFW_KEY_W])
	{
		camera.ProcessKeyboard(FORWARD, deltaTime);
	}
	if (keys[GLFW_KEY_S])
	{
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	}
	if (keys[GLFW_KEY_A])
	{
		camera.ProcessKeyboard(LEFT, deltaTime);
	}
	if (keys[GLFW_KEY_D])
	{
		camera.ProcessKeyboard(RIGHT, deltaTime);
	}
	/*if (keys[GLFW_KEY_ESCAPE])
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}*/
}

void key_press_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if ((key == GLFW_KEY_ESCAPE) && (action == GLFW_PRESS))
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	if (action == GLFW_PRESS)
	{
		keys[key] = GLFW_TRUE;  //这里一定一定不能写成“==“，否则  按键WSAD按键失效！！！！！！！
	}
	else if (action == GLFW_RELEASE)
	{
		keys[key] = GLFW_FALSE;    //这里一定一定不能写成“==“，否则  按键WSAD按键失效！！！！！！！
	}
}

void mouse_move_callback(GLFWwindow* window, double xPos, double yPos)
{
	if (b_firstMouse)
	{
		lastLocX = xPos;
		lastLocY = yPos;
		b_firstMouse = GL_FALSE;
	}

	float xOffset = xPos - lastLocX;
	float yOffset = lastLocY - yPos;
	lastLocX = xPos;
	lastLocY = yPos;

	camera.ProcessMouseMovement(xOffset, yOffset);

}

void mouse_scroll_callback(GLFWwindow* window, double xPos, double yPos)
{
	camera.ProcessMouseScroll(yPos);
}


void init(GLFWwindow* window)
{
	/*.vert - 顶点着色器
	  .tesc - 曲面细分控制着色器
	  .tese - 曲面细分评估着色器
	  .geom - 几何着色器
	  .frag - 片段着色器
		.comp - 计算着色器*/
	renderingProgram = Utils::createShaderProgram("vertShader.vert", "tessCShader.tesc", "tessEShader.tese", "fragShader.frag");
	cameraX = 0.f, cameraY = 0.f, cameraZ = 4.f;
	terLocX = 0.f, terLocY = 0.f, terLocZ = 0.f;

	glfwGetFramebufferSize(window, &width, &height);
	aspect = (float)width / (float)height;
	pMat = glm::perspective(toRadians(45.f), aspect, 0.1f, 1000.f);

	floorTexture = Utils::loadTexture("floor_color.jpg");
	glGenVertexArrays(numVAOs, vao);
	glBindVertexArray(vao[0]);
}

void display(GLFWwindow* window, double currentTime)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.1f, 0.2f, 0.5f, 1.f);

	glUseProgram(renderingProgram);  //这句不能少，否则渲染不出曲面

	deltaTime = currentTime - lastFrame;
	lastFrame = currentTime;

	do_movement();
	//这句必须要有，否则鼠标中键失效
	pMat = glm::perspective(camera.Zoom, aspect, 0.1f, 1000.f);

	//没有这句，背景就没在相机视点上了,把圆环移到相机的位置
	//mMat = glm::translate(glm::mat4(1.f), glm::vec3(cameraX, cameraY, 4.5f));
	vMat = camera.GetViewMatrix();



	//vMat = glm::translate(glm::mat4(1.f), glm::vec3(-cameraX, -cameraY, -cameraZ));
	mMat = glm::translate(glm::mat4(1.f), glm::vec3(terLocX, terLocY, terLocZ));
	mMat = glm::rotate(mMat, toRadians(45.f), glm::vec3(1.f, 0.f, 0.f));  //mMat;
	mMat = glm::rotate(mMat, toRadians(30.f), glm::vec3(0.f, 1.f, 0.f));
	mvpMat = pMat * vMat * mMat;

	mvpLoc = glGetUniformLocation(renderingProgram, "mvp_matrix");

	glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(mvpMat));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, floorTexture);

	glFrontFace(GL_CCW);

	//指定补丁原语的参数, pname指定要修改的参数，并且必须为GL_PATCH_VERTICES。 value为pname指定的参数指定新值。
	//当pname为GL_PATCH_VERTICES时，value指定将用于构成单个面片基元的顶点数。修补程序基元由镶嵌细分控制着色器（如果存在）消耗，随后用于镶嵌细分。
	//使用glDrawArrays或类似函数指定图元时，每个面片均由参数控制点组成，每个控制点均由从enabeld顶点数组中提取的顶点表示。
	//参数必须大于零，并且小于或等于GL_MAX_PATCH_VERTICES的值。
	glPatchParameteri(GL_PATCH_VERTICES, 16);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);// GL_FILL);
	glDrawArrays(GL_PATCHES, 0, 16);
}

void window_size_callback(GLFWwindow* window, int newWidth, int newHeight)
{
	glViewport(0, 0, newWidth, newHeight);
	aspect = (float)newWidth / (float)newHeight;
	pMat = glm::perspective(toRadians(45.f), aspect, 0.1f, 1000.f);
}

int main(int argc, char** argv)
{
	int glfwState = glfwInit();
	if (glfwState == GLFW_FALSE)
	{
		cout << "GLFW initialize failed, invoke glfwInit()......Error file:" << __FILE__ << "......Error line:" << __LINE__ << endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_CORE_PROFILE, GLFW_OPENGL_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

	GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "Tessellation Bezier", nullptr, nullptr);
	if (!window)
	{
		cout << "GLFW create window failed, invoke glfwCreateWindow()......Error file:" << __FILE__ << "......Error line:" << __LINE__ << endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);

	

	int glewState = glewInit();
	if (glewState != GLEW_OK)
	{
		cout << "GLEW initialize failed, invoke glewInit()......Error file:" << __FILE__ << "......Error line:" << __LINE__ << endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwSetWindowSizeCallback(window, window_size_callback);
	glfwSetCursorPosCallback(window, mouse_move_callback);
	glfwSetScrollCallback(window, mouse_scroll_callback);
	glfwSetKeyCallback(window, key_press_callback);

	glfwSwapInterval(1);

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

	return 0;
}


