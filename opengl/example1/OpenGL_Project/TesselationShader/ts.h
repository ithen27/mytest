#pragma once

#include "../application.h"
#include "../lib/camera/Camera.h"

class TS : public Application {
public:
	TS() {};
	void init(GLFWwindow* window);
	void input(GLFWwindow* window);
	void render();
	void clear();
	void prepare();
	void randerCube(const Shader& shader);
	void randerPlane(const Shader& shader);

	std::string title = "TS";

	void updateWindowSize(int h, int w)
	{
		this->windowHeight = h;
		this->windowWidth = w;
	}
	int windowHeight;
	int windowWidth;
private:
	GLFWwindow* window;
	ImVec4 clear_color;
	unsigned int VBO, VAO, EBO;

	Shader* objectShader;
	Shader* lightingShader;
	Shader* depthShader;

	float eyeValue[3];
	float lightPosValue[3];

	// ��ת��Դ
	bool rotateLight = false;
	bool preProject = false;

	// camera
	Camera* camera;
	int viewType = 0;

	float ambientStrength = 0.1f;
	float specularStrength = 0.5f;
	float diffuseStrength = 1.0f;
	int shininess = 32;

	// ��Ӱ

	unsigned int SHADOW_WIDTH = 4096, SHADOW_HEIGHT = 4096; // ����������
	unsigned int depthMapFBO;
	unsigned int depthMap;
	float near_plane = 1.0f, far_plane = 7.5f;
};