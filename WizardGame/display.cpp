#include "display.h"
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <iostream>
#include <SDL2/SDL_mixer.h>

extern bool close;

Display::Display(int width, int height, const std::string& title)
{
    this->width = width;
    this->height = height;
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    m_window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height,SDL_WINDOW_OPENGL);
    m_glContext = SDL_GL_CreateContext(m_window);
    SDL_SetWindowGrab(m_window,SDL_TRUE);
    SDL_SetRelativeMouseMode(SDL_TRUE);

    GLenum status = glewInit();

    if (status != GLEW_OK) {
        std::cerr << "GLEW FAILED TO INITIALIZE" << std::endl;
    }

    m_isClosed = false;
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glCullFace(GL_BACK);
    SDL_Init(SDL_INIT_AUDIO);
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cout << "error\n";
    }
    Mix_Volume(-1, MIX_MAX_VOLUME / 50.0);
}

Display::~Display()
{
    SDL_GL_DeleteContext(m_glContext);
    SDL_DestroyWindow(m_window);
    SDL_Quit();
}
bool Display::isClosed() {
    return m_isClosed;
}
void Display::init(int width, int height, const std::string& title) {
    SDL_GL_DeleteContext(m_glContext);
    SDL_DestroyWindow(m_window);

    this->width = width;
    this->height = height;
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    m_window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);
    m_glContext = SDL_GL_CreateContext(m_window);
    SDL_SetWindowGrab(m_window, SDL_TRUE);
    SDL_SetRelativeMouseMode(SDL_TRUE);

    GLenum status = glewInit();

    if (status != GLEW_OK) {
        std::cerr << "GLEW FAILED TO INITIALIZE" << std::endl;
    }

    m_isClosed = false;
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glCullFace(GL_BACK);
    SDL_Init(SDL_INIT_AUDIO);
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cout << "error\n";
    }
    Mix_Volume(-1, MIX_MAX_VOLUME / 2.0);
}
void Display::Clear(float r, float g, float b, float a) {
//    BindAsRenderTarget();
    glClearColor(r,g,b,a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    // SDL_WarpMouseInWindow(m_window,400,300);
}
void Display::DrawText() {
}
void Display::Update() {
    m_isClosed = close;

    SDL_GL_SwapWindow(m_window);
    SDL_Event e;
    while(SDL_PollEvent(&e)) {
        if (e.type==SDL_QUIT) {
            m_isClosed = true;
        }
    }
}
void Display::BindAsRenderTarget() {
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glViewport(0,0,width,height);
}
