#include <iostream>
#include <fstream>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "SkeletalAnimation/Types.h"
#include "SkeletalAnimation/SkeletalModelSerializer.h"
#include "SkeletalAnimation/SkeletalModel.h"
#include "AssimpConverter.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "display.h"
#include "mesh.h"
#include "shader.h"
#include "texture.h"
#include "transform.h"
#include "camera.h"
#include "object.h"
#include "world.h"
#define LUAINTF_LINK_LUA_COMPILED_IN_CXX 1
#include "luaLibrary.h"
#include "GraphicsComponent.h"
#include "NpcComponent.h"
#include "Component.h"
#include "postProcessing.h"
#include <qu3e/q3.h>
#include "FrameBuffer.h"
#include "imgui_sdl.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_sdl.h"
extern "C" {
#include "audio/audio.h"
}
#include "skybox.h"
#include "Globals.h"

//#include <ft2build.h>
//#include <freetype/freetype.h>
//#include <freetype/ftglyph.h>
//#include <freetype/ftoutln.h>
//#include <freetype/fttrigon.h>
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#undef main

extern std::vector<std::string> mods;
extern bool close;
extern bool resetFramebuffer;
extern bool resetWindow;
//extern Display display;
//extern FrameBuffer postProcessing;
//extern void initWindow();
int main()
{
    std::cout << "starting program\n";
    bool Left = false;
    bool Right = false;
    // Vertex vertices[] = {Vertex(glm::vec3(-0.5f,-0.5f,0), glm::vec2(0.0,0.0)),
    //                      Vertex(glm::vec3(0.0f,0.5f,0),      glm::vec2(0.5,1.0)),
    //                      Vertex(glm::vec3(0.5f,-0.5f,0),  glm::vec2(1.0,0.0))
    //                     };
    unsigned int indices[] = {0,1,2};
    
    Display display(Width,Height, "Wizard Game!");
    float counter = 0.0f;
    mods.push_back("testMod");
    World world;
    std::cout << "about to initialise Framebuffer\n";
    FrameBuffer postProcessing;
    //display.init(Width, Height, "Wizard Game!");
    initAudio();
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    ImGui_ImplSDL2_InitForOpenGL(display.m_window, display.m_glContext);
    const char* glsl_version = "#version 130";
    ImGui_ImplOpenGL3_Init(glsl_version);
    glViewport(0, 0, 1920, 1080);
    Skybox skybox;

    std::cout << "finished init\n\n\n";

    std::fstream myfile("./res/save.dat", std::ios_base::in);
    int a;
    while (myfile >> a)
    {
        printf("%f ", a);
    }
    std::cout << "\n\n\n\n\n";
    while(!display.m_isClosed) {
        //resize window if needed
        if (resetFramebuffer == true) {
            postProcessing = FrameBuffer();
            resetFramebuffer = false;
        }
        if (resetWindow == true) {
            //display.init(Width, Height, "Wizard Game!");
            SDL_SetWindowSize(display.m_window, Width, Height);
            IMGUI_CHECKVERSION();
            ImGui::CreateContext();
            ImGuiIO& io = ImGui::GetIO();
            ImGui_ImplSDL2_InitForOpenGL(display.m_window, display.m_glContext);
            const char* glsl_version = "#version 130";
            ImGui_ImplOpenGL3_Init(glsl_version);
            resetWindow = false;
        }



        world.Update();
        postProcessing.BindFrameBuffer();

        display.Clear(0.294f,0.463f,0.639f,1.0f);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame(display.m_window);
        ImGui::NewFrame();
        skybox.Draw(world.testMap.eSystem.getMainCam());

        world.Draw();
        counter+=0.01f;
        display.Update();
        postProcessing.RenderFrameBuffer();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
    endAudio();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
    return 0;
}
