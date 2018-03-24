#include "FrameBuffer.h"


void FrameBuffer::Create() {
    m_RendererId = 0;
    m_FrameBuffer = 0;
    m_RendererId = new unsigned int[1];
    m_IntermediateRendererId = new unsigned int[1];
    GLCall(glGenTextures(m_FrameBufferCreateInfo.RenderTargetCount, m_RendererId));
    GLCall(glGenTextures(m_FrameBufferCreateInfo.RenderTargetCount, m_IntermediateRendererId));

    GLCall(glGenFramebuffers(1, &m_FrameBuffer));
    if(m_FrameBufferCreateInfo.MultiSampleCount > 1)
    {
        GLCall(glGenFramebuffers(1, &m_IntermediateFrameBuffer));
    }

    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer));

    for (int i = 0; i < m_FrameBufferCreateInfo.RenderTargetCount; i++) {
        GLenum targetType = 0;
        if(m_FrameBufferCreateInfo.MultiSampleCount > 1)
            targetType = GL_TEXTURE_2D_MULTISAMPLE;
        else
            targetType = GL_TEXTURE_2D;

        GLCall(glBindTexture(targetType, m_RendererId[i]));


        if(m_FrameBufferCreateInfo.MultiSampleCount > 1) {
            GLCall(glTexImage2DMultisample(targetType,
                                           m_FrameBufferCreateInfo.MultiSampleCount,
                                           m_FrameBufferCreateInfo.InternalFormats[i],
                                           m_FrameBufferCreateInfo.Width,
                                           m_FrameBufferCreateInfo.Height,
                                           GL_TRUE));
        }
        else {
            GLCall(glTexImage2D(targetType, 0,
                                m_FrameBufferCreateInfo.InternalFormats[i],
                                m_FrameBufferCreateInfo.Width,
                                m_FrameBufferCreateInfo.Height,
                                0,
                                m_FrameBufferCreateInfo.Formats[i],
                                m_FrameBufferCreateInfo.Types[i],
                                nullptr));
        }

        if(m_FrameBufferCreateInfo.MultiSampleCount <= 1) {
            GLCall(glTexParameteri(targetType, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
            GLCall(glTexParameteri(targetType, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

            GLCall(glTexParameteri(targetType, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER));
            GLCall(glTexParameteri(targetType, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER));
        }

        GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER,
                                      m_FrameBufferCreateInfo.Attachments[i],
                                      targetType,
                                      m_RendererId[i],
                                      0));

        if(m_FrameBufferCreateInfo.Borders[i]) {
            float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
            GLCall(glTexParameterfv(targetType, GL_TEXTURE_BORDER_COLOR, borderColor));
        }
    }

    if(m_FrameBufferCreateInfo.UseDepthRenderBuffer) {
        unsigned int rboDepth;
        glGenRenderbuffers(1, &rboDepth);
        glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);

        if(m_FrameBufferCreateInfo.MultiSampleCount > 1) {
            glRenderbufferStorageMultisample(GL_RENDERBUFFER, m_FrameBufferCreateInfo.MultiSampleCount, GL_DEPTH24_STENCIL8, m_FrameBufferCreateInfo.Width, m_FrameBufferCreateInfo.Height);
        }
        else {
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_FrameBufferCreateInfo.Width, m_FrameBufferCreateInfo.Height);
        }

        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
    }

    if(m_FrameBufferCreateInfo.MultiSampleCount > 1)
    {
        GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_IntermediateFrameBuffer));
        for (int i = 0; i < m_FrameBufferCreateInfo.RenderTargetCount; i++) {
            GLCall(glBindTexture(GL_TEXTURE_2D, m_IntermediateRendererId[i]));


            GLCall(glTexImage2D(GL_TEXTURE_2D,
                                0,
                                m_FrameBufferCreateInfo.InternalFormats[i],
                                m_FrameBufferCreateInfo.Width,
                                m_FrameBufferCreateInfo.Height,
                                0,
                                m_FrameBufferCreateInfo.Formats[i],
                                m_FrameBufferCreateInfo.Types[i],
                                nullptr));

            GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
            GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

            GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER));
            GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER));

            GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, m_FrameBufferCreateInfo.Attachments[i], GL_TEXTURE_2D, m_IntermediateRendererId[i], 0));

            if(m_FrameBufferCreateInfo.Borders[i]) {
                float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
                GLCall(glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor));
            }
        }
    }


    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;

    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

FrameBuffer::FrameBuffer(FrameBufferCreateInfo createInfo) {
    m_FrameBufferCreateInfo = createInfo;
    Create();
}


void FrameBuffer::BindAsFrameBuffer() {
    GLCall(glViewport(0, 0,
                      m_FrameBufferCreateInfo.Width,
                      m_FrameBufferCreateInfo.Height));

    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer));

    glDrawBuffers(
            m_FrameBufferCreateInfo.RenderTargetCount,
            m_FrameBufferCreateInfo.Attachments);
}

void FrameBuffer::BindAsTexture(int frameBufferId, int slot)
{

    if(m_FrameBufferCreateInfo.MultiSampleCount > 1)
    {
        GLint currentFbo;
        glGetIntegerv(GL_FRAMEBUFFER_BINDING, &currentFbo);
        glBindFramebuffer(GL_READ_FRAMEBUFFER, m_FrameBuffer);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_IntermediateFrameBuffer);

        glBlitFramebuffer(0, 0,
                          m_FrameBufferCreateInfo.Width,
                          m_FrameBufferCreateInfo.Height,
                          0, 0,
                          m_FrameBufferCreateInfo.Width,
                          m_FrameBufferCreateInfo.Height,
                          GL_COLOR_BUFFER_BIT, GL_NEAREST);

        glBindFramebuffer(GL_FRAMEBUFFER, static_cast<GLuint>(currentFbo));
        GLCall(glActiveTexture(GL_TEXTURE0 + slot));
        GLCall(glBindTexture(GL_TEXTURE_2D, m_IntermediateRendererId[frameBufferId]));
    }
    else {
        GLCall(glActiveTexture(GL_TEXTURE0 + slot));
        GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererId[frameBufferId]));
    }

}

FrameBufferCreateInfo::FrameBufferCreateInfo() {
    Width = 0;
    Height = 0;
    UseDepthRenderBuffer = true;
    MultiSampleCount = 0;
    RenderTargetCount = 1;
    Borders = new bool[1] {false};
    InternalFormats = new GLenum[1] {GL_RGB};
    Formats = new GLenum[1] {GL_RGB};
    Types = new GLenum[1] {GL_UNSIGNED_BYTE};
    Attachments = new GLenum[1] {GL_COLOR_ATTACHMENT0};
}
