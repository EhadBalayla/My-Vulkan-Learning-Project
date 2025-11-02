#include "Engine.h"

Window Engine::m_Window;
Renderer Engine::m_Renderer;
Camera Engine::cam;

float Engine::DeltaTime = 0.01f;
float Engine::lastX = 0.0f;
float Engine::lastY = 0.0f;
bool Engine::firstMouse = true;



void Engine::Init() {
	m_Window.InitializeGLFW();
	m_Window.InitializeWindow();
	glfwSetCursorPosCallback(m_Window.getGLFWwindow(), Engine::Mouse_Callback);
	glfwSetInputMode(m_Window.getGLFWwindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	m_Renderer.Init();

	shader1.LoadTexture("Assets/M16A1_Texture.png");
	shader1.LoadShader("defaultVert.spv", "defaultFrag.spv");
	firstMesh.LoadMesh("Assets/M16A1_NoBones.fbx");

	cam.Position += glm::vec3(0.0f, -0.25f, 3.0f);

	GameLoop();
}
void Engine::GameLoop() {
	while (!m_Window.ShouldWindowClose()) {
		m_Window.pollEvents();

		ProcessInput();





		//start rendering the frame
		m_Renderer.startDrawing();




		shader1.updateUniformBuffers(glm::mat4(1.0f));

		shader1.Bind(m_Renderer.getBufferAt(m_Renderer.currentFrame));
		firstMesh.Draw();


		m_Renderer.endDrawing();
		//finish rendering the frame
	}

	Terminate();
}
void Engine::Terminate() {
	m_Renderer.earlyTerminate();

	shader1.UnloadTexture();
	shader1.UnloadShader();
	firstMesh.UnloadMesh();

	m_Renderer.Terminate();
	m_Window.Terminate();
}





void Engine::Mouse_Callback(GLFWwindow* window, double xposIn, double yposIn) {
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	cam.ProcessMouseMovement(xoffset, yoffset);
}
void Engine::ProcessInput() {
	if (glfwGetKey(m_Window.getGLFWwindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(m_Window.getGLFWwindow(), true);

	if (glfwGetKey(m_Window.getGLFWwindow(), GLFW_KEY_W) == GLFW_PRESS)
		cam.ProcessKeyboard(FORWARD, DeltaTime);
	if (glfwGetKey(m_Window.getGLFWwindow(), GLFW_KEY_S) == GLFW_PRESS)
		cam.ProcessKeyboard(BACKWARD, DeltaTime);
	if (glfwGetKey(m_Window.getGLFWwindow(), GLFW_KEY_A) == GLFW_PRESS)
		cam.ProcessKeyboard(LEFT, DeltaTime);
	if (glfwGetKey(m_Window.getGLFWwindow(), GLFW_KEY_D) == GLFW_PRESS)
		cam.ProcessKeyboard(RIGHT, DeltaTime);

	if (glfwGetKey(m_Window.getGLFWwindow(), GLFW_KEY_SPACE) == GLFW_PRESS)
		cam.ProcessKeyboard(UP, DeltaTime);
	if(glfwGetKey(m_Window.getGLFWwindow(), GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		cam.ProcessKeyboard(DOWN, DeltaTime);
}