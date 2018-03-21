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

unsigned int quadVAO, quadVBO;
void RenderScreenQuad() {
    if(quadVAO == 0) {
        float quadVertices[] = {
                // positions        // texture Coords
                -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
                -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
                 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
                 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        };
        GLCall(glGenVertexArrays(1, &quadVAO));
        GLCall(glGenBuffers(1, &quadVBO));
        GLCall(glBindVertexArray(quadVAO));
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, quadVBO));
        GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW));

        GLCall(glEnableVertexAttribArray(0));
        GLCall(glEnableVertexAttribArray(1));

        GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0));
        GLCall(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float))));

        GLCall(glDisableVertexAttribArray(0));
        GLCall(glDisableVertexAttribArray(1));
    }

    GLCall(glEnableVertexAttribArray(0));
    GLCall(glEnableVertexAttribArray(1));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, quadVBO));
    GLCall(glBindVertexArray(quadVAO));

    GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0));
    GLCall(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float))));

    GLCall(glDrawArrays(GL_TRIANGLE_STRIP, 0, 4));

    GLCall(glDisableVertexAttribArray(0));
    GLCall(glDisableVertexAttribArray(1));
}

int main()
{

	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;


	int winWidth = 1920;
	int winHeight = 1080;

    int multiSampleCount = 4;

    bool bloom = true;
    float exposure = 1.0f;
    float gamma = 1.4f;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(winWidth, winHeight, "3DGameEngine", nullptr, nullptr);
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
	camera.Position = { 0, 3, 0 };

    Shader screenShader("res/FinalScreen.glsl");
    Shader shader("res/Main.glsl");
    Shader depthMapShader("res/DepthMap.glsl");
    Shader blurShader("res/BlurShader.glsl");

    Model* sponzaModel = Model::GetModel("res/test.obj");



    InputManager::Init(window);

	DirectionalLight dirLight({1,1,1}, { glm::radians(170.0f), glm::radians(0.0f), glm::radians(0.0f) }, 0.5f, 0.6f);


    FrameBuffer depthBuffer = FrameBuffer(2048, 2048, 1, true, GL_DEPTH_COMPONENT , GL_DEPTH_COMPONENT, GL_FLOAT, GL_DEPTH_ATTACHMENT);

    bool* borders = new bool[2] {false, false};
    GLenum* internalFormats = new GLenum[2] {GL_RGBA16F, GL_RGBA16F};
    GLenum* formats = new GLenum[2] {GL_RGB, GL_RGB};
    GLenum* types = new GLenum[2] {GL_UNSIGNED_BYTE, GL_UNSIGNED_BYTE};
    GLenum* attachments = new GLenum[2] {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
    FrameBuffer screenBuffer = FrameBuffer(winWidth, winHeight, multiSampleCount, 2, borders, internalFormats, formats, types, attachments);

    FrameBuffer* pingPongBuffers = new FrameBuffer[2]{
            FrameBuffer(winWidth, winHeight, 1, false, GL_RGB16F, GL_RGB, GL_FLOAT, GL_COLOR_ATTACHMENT0),
            FrameBuffer(winWidth, winHeight, 1, false, GL_RGB16F, GL_RGB, GL_FLOAT, GL_COLOR_ATTACHMENT0),
    };

    float someNumber = 0;


    glEnable(GL_MULTISAMPLE);

    while (!glfwWindowShouldClose(window))
	{

        if(InputManager::GetKey(GLFW_KEY_Q)) {
            camera.Rotation -= Vector3f(0, 2, 0);
        }
        if(InputManager::GetKey(GLFW_KEY_E)) {
            camera.Rotation += Vector3f(0, 2, 0);
        }

        if(InputManager::GetKeyDown(GLFW_KEY_R)) {
            shader.Reload();
        }

        Vector3f change;
        float speed = 0.2f;

        if(InputManager::GetKey(GLFW_KEY_W))
        {
            change.x -= cos(glm::radians(camera.Rotation.y + 90)) *speed;
            change.z -= sin(glm::radians(camera.Rotation.y + 90)) *speed;
        }
        if(InputManager::GetKey(GLFW_KEY_S))
        {
            change.x += cos(glm::radians(camera.Rotation.y + 90)) *speed;
            change.z += sin(glm::radians(camera.Rotation.y + 90)) *speed;
        }
        if(InputManager::GetKey(GLFW_KEY_A))
        {
            change.x += -cos(glm::radians(camera.Rotation.y)) *speed;
            change.z += -sin(glm::radians(camera.Rotation.y)) *speed;
        }
        if(InputManager::GetKey(GLFW_KEY_D))
        {
            change.x += cos(glm::radians(camera.Rotation.y)) *speed;
            change.z += sin(glm::radians(camera.Rotation.y)) *speed;
        }
        camera.Position += change;

		scene.Step();

        Renderer::Start3D();

        /* Render Depth buffer */
        glEnable(GL_DEPTH_TEST);

        depthBuffer.BindAsFrameBuffer();
        glClear(GL_DEPTH_BUFFER_BIT);

        float halfShadowArea = 100;

        glm::mat4 lightProjection = glm::ortho(-halfShadowArea, halfShadowArea, -halfShadowArea, halfShadowArea, -halfShadowArea, halfShadowArea);
        Matrix4f lightView;
        lightView = glm::rotate(lightView, dirLight.GetRotation().x, { 1, 0, 0 });
        lightView = glm::rotate(lightView, dirLight.GetRotation().y, { 0, 1, 0 });
        lightView = glm::rotate(lightView, dirLight.GetRotation().z, { 0, 0, 1 });
        lightView = glm::translate(lightView, -camera.Position);

        glm::mat4 lightSpaceMatrix = (lightProjection * lightView);

        depthMapShader.Bind();
        depthMapShader.SetUniform4fv("u_LightSpaceMatrix", lightSpaceMatrix);

        Matrix4f matrix;
        matrix = glm::translate(matrix, {0,0,0});
        matrix = glm::scale(matrix, { 1.0f,1.0f,1.0f });
        //matrix = glm::scale(matrix, { 1.0f,1.0f,1.0f });
        depthMapShader.SetUniform4fv("u_ModelMatrix", matrix);

        sponzaModel->Draw(shader);

        /* Render Normal */
        screenBuffer.BindAsFrameBuffer();
        glClearColor(0, 0.5, 0.8, 1);
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        shader.Bind();
		shader.SetUniform4fv("u_LightSpaceMatrix", lightSpaceMatrix);
		shader.SetUniform4fv("u_ViewMatrix", camera.GetViewMatrix());


		shader.SetUniform4fv("u_ProjMatrix", glm::perspective(glm::radians(60.0f), winWidth / (float)winHeight, 0.1f, 10000.0f));
		shader.SetUniform3f("u_ViewPos", camera.Position);

        dirLight.Bind(shader);

        shader.SetUniform1f("u_SpecStrength", 0.5f);
        shader.SetUniform1f("u_SpecPow", 32);
        shader.SetUniform2f("u_Tiling", {0.4f,0.4f});
        shader.SetTexture("u_ShadowMap", depthBuffer, 0);

        shader.SetUniform4fv("u_ModelMatrix", matrix);


        /* TEMP */
        sponzaModel->Draw(shader);


        /* Post Processing */
        glDisable(GL_DEPTH_TEST);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, winWidth, winHeight);
        // clear all relevant buffers
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // set clear color to white (not really necessery actually, since we won't be able to see behind the quad anyways)
        glClear(GL_COLOR_BUFFER_BIT);



        bool horizontal = true, first_iteration = true;
        unsigned int amount = 10;
        blurShader.Bind();
        for (unsigned int i = 0; i < amount; i++)
        {
            pingPongBuffers[horizontal].BindAsFrameBuffer();
            blurShader.SetUniform1i("u_Horizontal", horizontal);
            blurShader.SetTexture("u_Image", first_iteration ? screenBuffer : pingPongBuffers[!horizontal],
                                  first_iteration ? 1 : 0);  // bind texture of other framebuffer (or scene if first iteration)
            RenderScreenQuad();
            horizontal = !horizontal;
            if (first_iteration)
                first_iteration = false;
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);


        /* draw on screen */
        screenShader.Bind();
        screenShader.SetTexture("u_Scene", screenBuffer, 0);
        screenShader.SetTexture("u_BloomBlur", pingPongBuffers[!horizontal], 0);
        screenShader.SetUniform1i("u_Bloom", (int)bloom);
        screenShader.SetUniform1f("u_Exposure", exposure);
        screenShader.SetUniform1f("u_Gamma", gamma);
        RenderScreenQuad();

		InputManager::Update();


		glfwSwapBuffers(window);
		glfwPollEvents();

	}

	return 0;
}

