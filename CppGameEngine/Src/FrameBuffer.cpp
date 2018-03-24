#include "FrameBuffer.h"


void FrameBuffer::Create(int width, int height, bool useDepthRenderbuffer, int multiSampleCount, int RenderTargetCount,  bool* borders, GLenum* internalFormats, GLenum* formats, GLenum* types, GLenum* attachments) {
    m_RendererId = 0;
    m_FrameBuffer = 0;
    m_Width = width;
    m_Height = height;
    m_RenderTargetCount = RenderTargetCount;
    m_RendererId = new unsigned int[1];
    m_IntermediateRendererId = new unsigned int[1];
    m_InternalFormats = internalFormats;
    m_Formats = formats;
    m_Types = types;
    m_Attachments = attachments;
    m_MultiSampleCount = multiSampleCount;
    GLCall(glGenTextures(RenderTargetCount, m_RendererId));
    GLCall(glGenTextures(RenderTargetCount, m_IntermediateRendererId));

    GLCall(glGenFramebuffers(1, &m_FrameBuffer));
    if(multiSampleCount > 1)
    {
        GLCall(glGenFramebuffers(1, &m_IntermediateFrameBuffer));
    }

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

        if(borders[i]) {
            float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
            GLCall(glTexParameterfv(targetType, GL_TEXTURE_BORDER_COLOR, borderColor));
        }
    }

    if(useDepthRenderbuffer) {
        unsigned int rboDepth;
        glGenRenderbuffers(1, &rboDepth);
        glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);

        if(multiSampleCount > 1) {
            glRenderbufferStorageMultisample(GL_RENDERBUFFER, multiSampleCount, GL_DEPTH24_STENCIL8, m_Width, m_Height);
        }
        else {
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_Width, m_Height);
        }

        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
    }

    if(multiSampleCount > 1)
    {
        GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_IntermediateFrameBuffer));
        for (int i = 0; i < RenderTargetCount; i++) {
            GLCall(glBindTexture(GL_TEXTURE_2D, m_IntermediateRendererId[i]));


            GLCall(glTexImage2D(GL_TEXTURE_2D, 0, internalFormats[i], width, height, 0, formats[i], types[i], nullptr));
            GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
            GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

            GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER));
            GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER));

            GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, attachments[i], GL_TEXTURE_2D, m_IntermediateRendererId[i], 0));

            if(borders[i]) {
                float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
                GLCall(glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor));
            }
        }
    }


    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;

    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

FrameBuffer::FrameBuffer(int width, int height, bool useDepthRenderbuffer, int multiSampleCount, bool border, GLenum internalFormat, GLenum format, GLenum type, GLenum attachment) {
    auto * borders = new bool[1] {border};
    auto * internalFormats = new GLenum[1] {internalFormat};
    auto * formats = new GLenum[1] {format};
    auto * types = new GLenum[1] {type};
    auto * attachments = new GLenum[1] {attachment};
    Create(width, height,useDepthRenderbuffer, multiSampleCount, 1, borders, internalFormats, formats, types, attachments);

}

FrameBuffer::FrameBuffer(int width, int height, bool useDepthRenderbuffer, int multiSampleCount, int RenderTargetCount, bool* border, GLenum* internalFormat, GLenum* format, GLenum* type, GLenum* attachment) {
    Create(width, height, useDepthRenderbuffer, multiSampleCount, RenderTargetCount, border, internalFormat, format, type, attachment);
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
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_IntermediateFrameBuffer);
        glBlitFramebuffer(0, 0, m_Width, m_Height, 0, 0, m_Width, m_Height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
        glBindFramebuffer(GL_FRAMEBUFFER, static_cast<GLuint>(currentFbo));
        GLCall(glActiveTexture(GL_TEXTURE0 + slot));
        GLCall(glBindTexture(GL_TEXTURE_2D, m_IntermediateRendererId[frameBufferId]));
    }
    else {
        GLCall(glActiveTexture(GL_TEXTURE0 + slot));
        GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererId[frameBufferId]));
    }

}