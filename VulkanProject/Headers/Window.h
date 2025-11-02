#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
class Window
{
public:
	void InitializeGLFW();
	void InitializeWindow();
	GLFWwindow* getGLFWwindow();
	bool ShouldWindowClose();
	void pollEvents();
	void Terminate();
private:
	GLFWwindow* m_GLFWwindow;

	static void resizeCallback(GLFWwindow* window, int width, int height);
};

