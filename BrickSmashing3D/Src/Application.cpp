// BrickSmashing3D.cpp : Defines the entry point for the console application.
//

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Common.h"
#include "Camera.h"
#include "Shader.h"
#include <glm/gtc/matrix_transform.hpp>
#include <lighting/DirectionalLight.h>
#include "vendor/physics/q3.h"
#include "InputManager.h"
#include "Renderer.h"
#include "Model.h"

q3Scene scene(1.0f / 60.0f);

int main()
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;


	int winWidth = 1920;
	int winHeight = 1080;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(winWidth, winHeight, "Platformer", nullptr, nullptr);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK)
		std::cout << "ERROR!" << std::endl;

	std::cout << glGetString(GL_VERSION) << std::endl;


	Camera camera;
	camera.SetPosition({ 0, 5, 0 });

	Shader shader("res/Main.shader");
    Shader depthMapShader("res/DepthMap.shader");
    Model* sponzaModel = Model::GetModel("res/sponza.obj");
    Texture baseTex = Texture("res/textures/background.tga");


	glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

	InputManager::Init(window);

	DirectionalLight dirLight({1,1,1}, { glm::radians(50.0f), glm::radians(-45.0f), glm::radians(0.0f) }, 0.5f, 0.6f);

    DepthMap depthBuffer = DepthMap();

	float xVal = 90.0f;
    float yVal = 0.0f;
	while (!glfwWindowShouldClose(window))
	{

        dirLight.SetRotation({glm::radians(xVal), glm::radians(0.0f), glm::radians(0.0f)});


		if(InputManager::GetKey(GLFW_KEY_C)) {
			xVal--;
		}
		if(InputManager::GetKey(GLFW_KEY_V)) {
			xVal++;
		}

        if(InputManager::GetKey(GLFW_KEY_Q)) {
            camera.Rotation -= Vector3f(0, 0.5f, 0);
        }
        if(InputManager::GetKey(GLFW_KEY_E)) {
            camera.Rotation += Vector3f(0, 0.5f, 0);
        }

        if(InputManager::GetKeyDown(GLFW_KEY_R)) {
            shader.Reload();
        }


        if(InputManager::GetKey(GLFW_KEY_W))
            camera.SetPosition({camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z - 0.5f});
        if(InputManager::GetKey(GLFW_KEY_S))
            camera.SetPosition({camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z + 0.5f});
        if(InputManager::GetKey(GLFW_KEY_A))
            camera.SetPosition({camera.GetPosition().x + 0.5f, camera.GetPosition().y, camera.GetPosition().z});
        if(InputManager::GetKey(GLFW_KEY_D))
            camera.SetPosition({camera.GetPosition().x - 0.5f, camera.GetPosition().y, camera.GetPosition().z});

		scene.Step();


        Renderer::Start3D();

        /* Render Depth buffer */
        depthBuffer.Bind();
        glClear(GL_DEPTH_BUFFER_BIT);

        float nearPlane = 1, farPlane = 50;
        glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, nearPlane, farPlane);
        Matrix4f lightView;
        lightView = glm::rotate(lightView, dirLight.GetRotation().x, { 1, 0, 0 });
        lightView = glm::rotate(lightView, dirLight.GetRotation().y, { 0, 1, 0 });
        lightView = glm::rotate(lightView, dirLight.GetRotation().z, { 0, 0, 1 });
        lightView = glm::translate(lightView, -camera.GetPosition());

        glm::mat4 lightSpaceMatrix = (lightProjection * lightView);

        depthMapShader.Bind();
        depthMapShader.SetUniform4fv("u_LightSpaceMatrix", lightSpaceMatrix);

        Matrix4f matrix;
        matrix = glm::translate(matrix, {0,0,0});
        matrix = glm::scale(matrix, { 1,1,1 });
        depthMapShader.SetUniform4fv("u_ModelMatrix", matrix);

        sponzaModel->Draw();

        /* Render Normal */
        glCullFace(GL_BACK);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, winWidth, winHeight);
        glClearColor(0, 0.5, 0.8, 1);
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        shader.Bind();
		shader.SetUniform4fv("u_LightSpaceMatrix", lightSpaceMatrix);
		shader.SetUniform4fv("u_ViewMatrix", camera.GetViewMatrix());
		shader.SetUniform4fv("u_ProjMatrix", glm::perspective(glm::radians(60.0f), winWidth / (float)winHeight, 0.1f, 100.0f));
		shader.SetUniform3f("u_ViewPos", camera.GetPosition());

        dirLight.Bind(shader);



        shader.SetUniform1i("u_UseSpecMap", false);
        shader.BindTexture("u_ShadowMap", depthBuffer);
        shader.BindTexture("u_Diffuse", baseTex);

        shader.SetUniform4fv("u_ModelMatrix", matrix);

        sponzaModel->Draw();

		InputManager::Update();


		glfwSwapBuffers(window);
		glfwPollEvents();

	}

	return 0;
}

