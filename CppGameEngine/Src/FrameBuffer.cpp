#include "FrameBuffer.h"


void FrameBuffer::Create(int width, int height, int multiSampleCount, int RenderTargetCount,  bool* borders, GLenum* internalFormats, GLenum* formats, GLenum* types, GLenum* attachments) {
    m_RendererId = 0;
    m_FrameBuffer = 0;
    m_Width = width;
    m_Height = height;
    m_RenderTargetCount = RenderTargetCount;
    m_RendererId = new unsigned int[1];
    m_IntermidiateRendererId = new unsigned int[1];
    m_InternalFormats = internalFormats;
    m_Formats = formats;
    m_Types = types;
    m_Attachments = attachments;
    m_MultiSampleCount = multiSampleCount;
    GLCall(glGenTextures(RenderTargetCount, m_RendererId));
    GLCall(glGenTextures(RenderTargetCount, m_IntermidiateRendererId));

    GLCall(glGenFramebuffers(1, &m_FrameBuffer));
    GLCall(glGenFramebuffers(1, &m_IntermdiateFrameBuffer));
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer));

    for (int i = 0; i < RenderTargetCount; i++) {
        GLenum targetType = 0;
        if(multiSampleCount > 1)
            targetType = GL_TEXTURE_2D_MULTISAMPLE;
        else
            targetType = GL_TEXTURE_2D;

        GLCall(glBindTexture(targetType, m_RendererId[i]));


        if(multiSampleCount > 1) {
            GLCall(glTexImage2DMultisample(targetType, multiSampleCount, internalFormats[i], width, height, GL_TRUE));
        }
        else {
            GLCall(glTexImage2D(targetType, 0, internalFormats[i], width, height, 0, formats[i], types[i], nullptr));
        }

        if(multiSampleCount <= 1) {
            GLCall(glTexParameteri(targetType, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
            GLCall(glTexParameteri(targetType, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

            GLCall(glTexParameteri(targetType, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER));
            GLCall(glTexParameteri(targetType, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER));
        }

        GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, attachments[i], targetType, m_RendererId[i], 0));

        if(borders[i] == true) {
            float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
            GLCall(glTexParameterfv(targetType, GL_TEXTURE_BORDER_COLOR, borderColor));
        }
    }

    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_IntermdiateFrameBuffer));
    for (int i = 0; i < RenderTargetCount; i++) {
        GLenum targetType = GL_TEXTURE_2D;

        GLCall(glBindTexture(targetType, m_IntermidiateRendererId[i]));


        GLCall(glTexImage2D(targetType, 0, internalFormats[i], width, height, 0, formats[i], types[i], nullptr));
        GLCall(glTexParameteri(targetType, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        GLCall(glTexParameteri(targetType, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

        GLCall(glTexParameteri(targetType, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER));
        GLCall(glTexParameteri(targetType, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER));

        GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, attachments[i], targetType, m_IntermidiateRendererId[i], 0));

        if(borders[i] == true) {
            float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
            GLCall(glTexParameterfv(targetType, GL_TEXTURE_BORDER_COLOR, borderColor));
        }
    }


    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;

    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

FrameBuffer::FrameBuffer(int width, int height, int multiSampleCount, bool border, GLenum internalFormat, GLenum format, GLenum type, GLenum attachment) {
    bool* borders = new bool[1] {border};
    GLenum* internalFormats = new GLenum[1] {internalFormat};
    GLenum* formats = new GLenum[1] {format};
    GLenum* types = new GLenum[1] {type};
    GLenum* attachments = new GLenum[1] {attachment};
    Create(width, height, multiSampleCount, 1, borders, internalFormats, formats, types, attachments);

}

FrameBuffer::FrameBuffer(int width, int height, int multiSampleCount, int RenderTargetCount, bool* border, GLenum* internalFormat, GLenum* format, GLenum* type, GLenum* attachment) {
    Create(width, height, multiSampleCount, RenderTargetCount, border, internalFormat, format, type, attachment);
}



void FrameBuffer::BindAsFrameBuffer() {
    GLCall(glViewport(0, 0, m_Width, m_Height));
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer));
    glDrawBuffers(m_RenderTargetCount, m_Attachments);
}

void FrameBuffer::BindAsTexture(int frameBufferId, int slot)
{

    if(m_MultiSampleCount > 1)
    {
        GLint currentFbo;
        glGetIntegerv(GL_FRAMEBUFFER_BINDING, &currentFbo);
        glBindFramebuffer(GL_READ_FRAMEBUFFER, m_FrameBuffer);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_IntermdiateFrameBuffer);
        glBlitFramebuffer(0, 0, m_Width, m_Height, 0, 0, m_Width, m_Height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
        glBindFramebuffer(GL_FRAMEBUFFER,currentFbo);
        GLCall(glActiveTexture(GL_TEXTURE0 + slot));
        GLCall(glBindTexture(GL_TEXTURE_2D, m_IntermidiateRendererId[frameBufferId]));
    }
    else {
        GLCall(glActiveTexture(GL_TEXTURE0 + slot));
        GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererId[frameBufferId]));
    }

}