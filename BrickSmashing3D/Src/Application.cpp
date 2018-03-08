// BrickSmashing3D.cpp : Defines the entry point for the console application.
//

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Common.h"
#include <iostream>
#include "Platform.h"
#include "Camera.h"
#include "Shader.h"
#include <glm/gtc/matrix_transform.hpp>
#include <lighting/DirectionalLight.h>
#include "Texture.h"
#include "Player.h"
#include "vendor/physics/q3.h"
#include "LevelLoader.h"
#include "InputManager.h"



std::vector<Platform*> platforms;
Player* player;
q3Scene scene(1.0f / 60.0f);
LevelLoader lvl = LevelLoader();

void UpdateEditorMode() {
    if(InputManager::GetKeyDown(GLFW_KEY_F)) {
        Platform* newPlat = new Platform(5, 1.2f, scene);
		newPlat->SetPosition(player->GetPosition());
        platforms.push_back(newPlat);
        lvl.Write("./res/test.file", platforms);
    }
}

int main()
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;


	int winWidth = 1280;
	int winHeight = 720;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(winWidth, winHeight, "Brick Smashing game", nullptr, nullptr);
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
	camera.SetPosition({ 0, 8, 20 });

	Shader shader("res/Main.shader");
	Texture platformTex("res/platform.jpg");
	Texture playerTex("res/player.jpg");

    player = new Player(scene);

	SceneFile file;

	lvl.Load("./res/test.file", file, scene);


	platforms = file.platforms;

	glEnable(GL_DEPTH_TEST);

	InputManager::Init(window);

    bool editorMode = false;

	DirectionalLight dirLight({1,1,1}, {0.0f, -0.4f, -0.5f}, 0.7f, 0.6f);

	std::cout << "Loaded " << platforms.size() << " platforms" << std::endl;

	while (!glfwWindowShouldClose(window))
	{
		if(!editorMode)
		    scene.Step();

		glClearColor(0, 0.5, 0.8, 1);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

		if (InputManager::GetKeyDown(GLFW_KEY_R))
		{
			shader.Reload();
			platformTex.Reload();
			player->Reload();

			for (int i = 0; i < platforms.size(); i++)
			{
				platforms[i]->Reload();
			}
		}


        if(InputManager::GetKeyDown(GLFW_KEY_T)) {
            editorMode = !editorMode;
            player->SetEditorMode(editorMode);
        }

        if(editorMode)
            UpdateEditorMode();

		player->Input();

		auto playerPos = player->GetPosition();
		camera.SetPosition({ playerPos.x, playerPos.y + 3, 10 });

		shader.Bind();
		shader.SetUniform4fv("u_ViewMatrix", camera.GetViewMatrix());
		shader.SetUniform4fv("u_ProjMatrix", glm::perspective(glm::radians(60.0f), winWidth / (float)winHeight, 0.1f, 100.0f));

        dirLight.Bind(shader);

		platformTex.Bind(0);

		for (int i = 0; i < platforms.size(); i++)
		{
			shader.SetUniform4fv("u_ModelMatrix", platforms[i]->GetModelMatrix());
			platforms[i]->Update();
			platforms[i]->Draw(shader);
		}


		shader.SetUniform4fv("u_ModelMatrix", player->GetModelMatrix());
		playerTex.Bind();
		player->Draw(shader);

		InputManager::Update();


		glfwSwapBuffers(window);
		glfwPollEvents();

	}

	return 0;
}

