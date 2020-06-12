#include "FrameBuffer.h"


void FrameBuffer::BindFrameBuffer() {
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebuffer);
    glEnable(GL_DEPTH_TEST);
    int h = glGetUniformLocation(ppShader.m_program, "horizontal");
    glUniform1i(h, horizontal);

}
void FrameBuffer::RenderFrameBuffer() {
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glDisable(GL_DEPTH_TEST);

    glClearColor(1.0f,1.0f,1.0f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    ppShader.Bind();
    int colorTexLocation = glGetUniformLocation(ppShader.m_program, "sampler");
    int depthTexLocation = glGetUniformLocation(ppShader.m_program, "depth");

    glUniform1i(colorTexLocation, 0);
    glUniform1i(depthTexLocation, 1);
    glActiveTexture(GL_TEXTURE0 + 0);
    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
    glActiveTexture(GL_TEXTURE0 + 1);
    glBindTexture(GL_TEXTURE_2D, depthbuffer);

    quad.Draw();
}
FrameBuffer::~FrameBuffer()
{
}

