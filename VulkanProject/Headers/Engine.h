#pragma once
#include "Window.h"
#include "Renderer.h"
#include "StaticMesh.h"
#include "Camera.h"

class Engine
{
public:
	static Window m_Window;
	static Renderer m_Renderer;

	static Camera cam;

	Shader shader1;
	StaticMesh firstMesh;


	const std::vector<Vertex> verticies = {
	{glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)},
	{glm::vec3(0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)},
	{glm::vec3(0.5f, 0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)},
	{glm::vec3(-0.5f, 0.5f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)}
	};
	const std::vector<uint32_t> indicies = {
	0, 1, 2, 2, 3, 0
	};

	void Init();
private:
	void GameLoop();
	void Terminate();

	static float DeltaTime;



	//mouse camera calculations
	static bool firstMouse;
	static float lastX;
	static float lastY;
	static void Mouse_Callback(GLFWwindow* window, double xposIn, double yposIn);

	//input camera calculations
	static void ProcessInput();
};

