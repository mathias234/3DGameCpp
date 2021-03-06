// BrickSmashing3D.cpp : Defines the entry point for the console application.
//

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Common.h"
#include "Camera.h"
#include "Shader.h"
#include "lighting/DirectionalLight.h"
#include "vendor/physics/q3.h"
#include "InputManager.h"
#include "Renderer.h"
#include "Model.h"
#include "UIRenderer.h"
#include "vendor/tinydir/tinydir.h"
#include "DirectoryInspector.h"
#include "GameObject.h"
#include <experimental/filesystem>
#include <glm/gtx/string_cast.hpp>


q3Scene scene(1.0f / 60.0f);

unsigned int quadVAO, quadVBO;

int winWidth = 1920;
int winHeight = 1080;
int shadowQuality = 8192;

int multiSampleCount = 4;

FrameBuffer* nearShadowBuffer;
FrameBuffer* farShadowBuffer;
FrameBuffer* screenBuffer;
FrameBuffer* pingPongBuffers;

void SetupFramebuffers() {
    FrameBufferCreateInfo nearShadowBufferCreateInfo;
    nearShadowBufferCreateInfo.Width = shadowQuality;
    nearShadowBufferCreateInfo.Height = shadowQuality;
    nearShadowBufferCreateInfo.UseDepthRenderBuffer = false;
    nearShadowBufferCreateInfo.Borders = new bool[1]{true};
    nearShadowBufferCreateInfo.InternalFormats = new GLenum[1]{GL_DEPTH_COMPONENT};
    nearShadowBufferCreateInfo.Formats = new GLenum[1]{GL_DEPTH_COMPONENT};
    nearShadowBufferCreateInfo.Types = new GLenum[1]{GL_FLOAT};
    nearShadowBufferCreateInfo.Attachments = new GLenum[1]{GL_DEPTH_ATTACHMENT};
    nearShadowBuffer = new FrameBuffer(nearShadowBufferCreateInfo);

    FrameBufferCreateInfo farShadowBufferCreateInfo;
    farShadowBufferCreateInfo.Width = shadowQuality;
    farShadowBufferCreateInfo.Height = shadowQuality;
    farShadowBufferCreateInfo.UseDepthRenderBuffer = false;
    farShadowBufferCreateInfo.Borders = new bool[1]{true};
    farShadowBufferCreateInfo.InternalFormats = new GLenum[1]{GL_DEPTH_COMPONENT};
    farShadowBufferCreateInfo.Formats = new GLenum[1]{GL_DEPTH_COMPONENT};
    farShadowBufferCreateInfo.Types = new GLenum[1]{GL_FLOAT};
    farShadowBufferCreateInfo.Attachments = new GLenum[1]{GL_DEPTH_ATTACHMENT};
    farShadowBuffer = new FrameBuffer(farShadowBufferCreateInfo);

    FrameBufferCreateInfo screenBufferCreateInfo;
    screenBufferCreateInfo.Width = winWidth;
    screenBufferCreateInfo.Height = winHeight;
    screenBufferCreateInfo.UseDepthRenderBuffer = true;
    screenBufferCreateInfo.MultiSampleCount = multiSampleCount;
    screenBufferCreateInfo.RenderTargetCount = 2;
    screenBufferCreateInfo.Borders = new bool[2]{false, false};
    screenBufferCreateInfo.InternalFormats = new GLenum[2]{GL_RGBA16F, GL_RGBA16F};
    screenBufferCreateInfo.Formats = new GLenum[2]{GL_RGB, GL_RGB};
    screenBufferCreateInfo.Types = new GLenum[2]{GL_UNSIGNED_BYTE, GL_UNSIGNED_BYTE};
    screenBufferCreateInfo.Attachments = new GLenum[2]{GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
    screenBuffer = new FrameBuffer(screenBufferCreateInfo);

    FrameBufferCreateInfo pingPongBuffersCreateInfo;
    pingPongBuffersCreateInfo.Width = winWidth;
    pingPongBuffersCreateInfo.Height = winHeight;
    pingPongBuffersCreateInfo.UseDepthRenderBuffer = false;
    pingPongBuffersCreateInfo.InternalFormats = new GLenum[1]{GL_RGB16F};
    pingPongBuffersCreateInfo.Formats = new GLenum[1]{GL_RGB};
    pingPongBuffersCreateInfo.Types = new GLenum[1]{GL_FLOAT};
    pingPongBuffersCreateInfo.Attachments = new GLenum[1]{GL_COLOR_ATTACHMENT0};

    pingPongBuffers = new FrameBuffer[2]{
            FrameBuffer(pingPongBuffersCreateInfo),
            FrameBuffer(pingPongBuffersCreateInfo),
    };
}

void RenderScreenQuad() {
    if (quadVAO == 0) {
        float quadVertices[] = {
                // positions        // texture Coords
                -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
                -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
                1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
                1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        };
        GLCall(glGenVertexArrays(1, &quadVAO));
        GLCall(glGenBuffers(1, &quadVBO));
        GLCall(glBindVertexArray(quadVAO));
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, quadVBO));
        GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW));

        GLCall(glEnableVertexAttribArray(0));
        GLCall(glEnableVertexAttribArray(1));

        GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) 0));
        GLCall(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) (3 * sizeof(float))));

        GLCall(glDisableVertexAttribArray(0));
        GLCall(glDisableVertexAttribArray(1));
    }

    GLCall(glEnableVertexAttribArray(0));
    GLCall(glEnableVertexAttribArray(1));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, quadVBO));
    GLCall(glBindVertexArray(quadVAO));

    GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) 0));
    GLCall(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) (3 * sizeof(float))));

    GLCall(glDrawArrays(GL_TRIANGLE_STRIP, 0, 4));

    GLCall(glDisableVertexAttribArray(0));
    GLCall(glDisableVertexAttribArray(1));
}

std::vector<GameObject*> gameObjects;

int main() {
    /* Settings */


    bool bloom = true;
    int bloomAmount = 15;
    float exposure = 1.0f;
    float gamma = 1.4f;

    float halfShadowArea = 20;

    bool useShadows = true;
    bool usePostProcessing = true;

    GLFWwindow *window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(winWidth, winHeight, "3DGameEngine", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

	if (!gladLoadGL()) {
		printf("Something went wrong!\n");
		exit(-1);
	}

    std::cout << glGetString(GL_VERSION) << " : " << glGetString(GL_VENDOR) << " : " << glGetString(GL_RENDERER)
              << std::endl;

    std::string glVersion = (const char *) glGetString(GL_VERSION);
    std::string glVendor = (const char *) glGetString(GL_VENDOR);
    std::string glRenderer = (const char *) glGetString(GL_RENDERER);

    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();

    UIRenderer uiRenderer{};
    uiRenderer.Init(window);

    // Setup style
    ImGui::StyleColorsDark();

    ImGuiStyle& style = ImGui::GetStyle();

    style.WindowRounding = 0;

    SetupFramebuffers();

    Camera camera;
    camera.Position = {0, 5, 0};
	camera.Rotation = {0, 0, 0};

    Shader *shader = new Shader("res/Main.glsl");
    Shader *screenShader = new Shader("res/FinalScreen.glsl");
    Shader *depthMapShader = new Shader("res/DepthMap.glsl");
    Shader *blurShader = new Shader("res/BlurShader.glsl");

    Model *testModel = Model::GetModel("res/test.obj");

    Texture *texture = Texture::GetTexture("res/textures/bricks_diff.jpg");

    for (int j = 0; j < testModel->SubmeshCount(); ++j) {
        testModel->GetMaterial(j)->Diffuse = texture;
    }

    GameObject* TestObj = new GameObject({0,0,0}, {1,1,1}, Quaternion(Vector3f(0,0,0)), testModel);
    gameObjects.push_back(TestObj);

    InputManager::Init(window);

    glEnable(GL_MULTISAMPLE);

    DirectionalLight dirLight({1, 1, 1}, {0, 0, 0}, 0.5f, 0.6f);

    bool show_demo_window = true;
    Vector3f directionalLightDir = {130.0f, 50.0f, 0.0f};
    float directionalLightStrength = 1;
    float ambientStrength = 0.6f;

    DirectoryInspector dirInspec;

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        dirLight.SetRotation(glm::radians(directionalLightDir));
        dirLight.SetDiffuseIntensity(directionalLightStrength);
        dirLight.SetAmbientIntensity(ambientStrength);


        if (InputManager::GetKey(GLFW_KEY_Q)) {
            camera.Rotation -= Vector3f(0, 2, 0);
        }
        if (InputManager::GetKey(GLFW_KEY_E)) {
            camera.Rotation += Vector3f(0, 2, 0);
        }

        if (InputManager::GetKeyDown(GLFW_KEY_R)) {
            shader->Reload();
        }

        Vector3f change = { 0.0f, 0.0f, 0.0f };
        float speed = 0.2f;

        if (InputManager::GetKey(GLFW_KEY_W)) {
            change.x -= cos(glm::radians(camera.Rotation.y + 90)) * speed;
            change.z -= sin(glm::radians(camera.Rotation.y + 90)) * speed;
        }
        if (InputManager::GetKey(GLFW_KEY_S)) {
            change.x += cos(glm::radians(camera.Rotation.y + 90)) * speed;
            change.z += sin(glm::radians(camera.Rotation.y + 90)) * speed;
        }
        if (InputManager::GetKey(GLFW_KEY_A)) {
            change.x += -cos(glm::radians(camera.Rotation.y)) * speed;
            change.z += -sin(glm::radians(camera.Rotation.y)) * speed;
        }
        if (InputManager::GetKey(GLFW_KEY_D)) {
            change.x += cos(glm::radians(camera.Rotation.y)) * speed;
            change.z += sin(glm::radians(camera.Rotation.y)) * speed;
        }


        camera.Position += change;

		//std::cout << glm::to_string(camera.Rotation) << std::endl;


        scene.Step();

        glm::mat4 nearLightProj = glm::ortho(-halfShadowArea, halfShadowArea, -halfShadowArea, halfShadowArea,
                                             -halfShadowArea, halfShadowArea);

        Matrix4f lightView = dirLight.GetLightMatrix();
        lightView = glm::translate(lightView, -camera.Position);

        glm::mat4 nearLightSpaceMatrix = (nearLightProj * lightView);
        glm::mat4 farightProj = glm::ortho(-halfShadowArea * 5, halfShadowArea * 5, -halfShadowArea * 5,
                                           halfShadowArea * 5, -halfShadowArea * 5, halfShadowArea * 5);
        glm::mat4 farLightSpaceMatrix = (farightProj * lightView);

        glEnable(GL_DEPTH_TEST);


        // Render shadow buffers
        if (useShadows) {
            /* Render Depth buffer */
            nearShadowBuffer->BindAsFrameBuffer();
            glClear(GL_DEPTH_BUFFER_BIT);

            /* Draw Near shadows */
            depthMapShader->Bind();
            depthMapShader->SetMatrix4("u_LightSpaceMatrix", nearLightSpaceMatrix);

            for (int j = 0; j < gameObjects.size(); ++j) {
                gameObjects[j]->Draw(*depthMapShader);
            }
            /* Draw Far shadows */
            farShadowBuffer->BindAsFrameBuffer();
            glClear(GL_DEPTH_BUFFER_BIT);


            depthMapShader->SetMatrix4("u_LightSpaceMatrix", farLightSpaceMatrix);

            for (int j = 0; j < gameObjects.size(); ++j) {
                gameObjects[j]->Draw(*depthMapShader);
            }
        }

        /* Render Normal */
        screenBuffer->BindAsFrameBuffer();
        glClearColor(0, 0.5, 0.8, 1);
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        shader->Bind();
        shader->SetMatrix4("u_NearLightSpaceMatrix", nearLightSpaceMatrix);
        shader->SetMatrix4("u_FarLightSpaceMatrix", farLightSpaceMatrix);
        shader->SetMatrix4("u_ViewMatrix", camera.GetViewMatrix());
		

		Matrix4f projMat = glm::perspective(glm::radians(60.0f), ((float)winWidth) / ((float)winHeight), 0.1f, 1000.0f);

		shader->SetMatrix4("u_ProjMatrix", projMat);

        shader->SetFloat3("u_ViewPos", camera.Position);

        dirLight.Bind(*shader);

        shader->SetFloat("u_SpecStrength", 4.5f);
        shader->SetFloat("u_SpecPow", 32);
        shader->SetFloat2("u_Tiling", {1.0f, 1.0f});
        shader->SetInt("u_UseShadows", useShadows);
        shader->SetTexture("u_NearShadowMap", *nearShadowBuffer, 0);
        shader->SetTexture("u_FarShadowMap", *farShadowBuffer, 0);


        for (int j = 0; j < gameObjects.size(); ++j) {
            gameObjects[j]->Draw(*shader);
        }

        /* Post Processing */
        pingPongBuffers[0].BindAsFrameBuffer();
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        pingPongBuffers[1].BindAsFrameBuffer();
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        bool horizontal = true, first_iteration = true;
        if (usePostProcessing) {
            blurShader->Bind();
            for (unsigned int i = 0; i < bloomAmount; i++) {
                pingPongBuffers[horizontal].BindAsFrameBuffer();
                blurShader->SetInt("u_Horizontal", horizontal);
                blurShader->SetTexture("u_Image", first_iteration ? *screenBuffer : pingPongBuffers[!horizontal],
                                       first_iteration ? 1
                                                       : 0);  // bind texture of other framebuffer (or screen if first iteration)
                RenderScreenQuad();
                horizontal = !horizontal;
                if (first_iteration)
                    first_iteration = false;
            }
        }

        /* draw on screen */
        glDisable(GL_DEPTH_TEST);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, winWidth, winHeight);
        // clear all relevant buffers
        glClearColor(1.0f, 1.0f, 1.0f,
                     1.0f); // set clear color to white (not really necessery actually, since we won't be able to see behind the quad anyways)
        glClear(GL_COLOR_BUFFER_BIT);

        screenShader->Bind();
        screenShader->SetTexture("u_Scene", *screenBuffer, 0);
        screenShader->SetTexture("u_BloomBlur", pingPongBuffers[!horizontal], 0);
        screenShader->SetInt("u_Bloom", (int) bloom);
        screenShader->SetFloat("u_Exposure", exposure);
        screenShader->SetFloat("u_Gamma", gamma);
        RenderScreenQuad();


        /* UI */
        uiRenderer.NewFrame(window);

        ImGui::Begin("Settings", &show_demo_window, ImGuiWindowFlags_NoNav);
        {
            ImGui::Text("Device info: ");
            ImGui::Text(("    Version: " + glVersion).c_str());
            ImGui::Text(("    Device: " + glRenderer).c_str());
            ImGui::Text(("    Vendor: " + glVendor).c_str());
            ImGui::Text("");


            ImGui::SliderFloat("Shadow Half Area", &halfShadowArea, 0.0f, 200.0f);
            ImGui::SliderFloat3("Light Direction", (float *) &directionalLightDir, 0, 360);
            ImGui::SliderFloat("Light Strength", &directionalLightStrength, 0, 10);
            ImGui::SliderFloat("Ambient Strength", &ambientStrength, 0, 10);

            ImGui::Checkbox("Use Shadows", &useShadows);
            if (useShadows) {
                ImGui::SliderInt("Shadow quality", &shadowQuality, 64, 8192);
                if (ImGui::Button("Apply shadow quality")) {
                    farShadowBuffer->FrameBufferSettings.Width = shadowQuality;
                    farShadowBuffer->FrameBufferSettings.Height = shadowQuality;
                    farShadowBuffer->Reload();

                    nearShadowBuffer->FrameBufferSettings.Width = shadowQuality;
                    nearShadowBuffer->FrameBufferSettings.Height = shadowQuality;
                    nearShadowBuffer->Reload();
                }
            }

            ImGui::Checkbox("Use Post Processing", &usePostProcessing);
            if (usePostProcessing) {
                ImGui::SliderInt("Bloom amount", &bloomAmount, 0, 50);
            }

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate,
                        ImGui::GetIO().Framerate);
        }
        ImGui::End();

        dirInspec.Draw(gameObjects);

        ImGui::Render();
        uiRenderer.RenderDrawData(ImGui::GetDrawData());

        InputManager::Update();

        glfwSwapBuffers(window);

    }

    return 0;
}

