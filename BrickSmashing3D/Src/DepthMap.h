//
// Created by matha on 14/03/2018.
//

#pragma once
#include "Common.h"

class DepthMap {
private:
    const unsigned int SHADOW_WIDTH = 2048, SHADOW_HEIGHT = 2048;
    unsigned int m_RendererId;
    unsigned int m_FrameBuffer;

public:
    DepthMap();
    void Bind();

    void BindTexture(int slot);
};


DepthMap::DepthMap() {
    m_RendererId = 0;
    m_FrameBuffer = 0;

    GLCall(glGenTextures(1, &m_RendererId));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererId));
    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
                 SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER));

    float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLCall(glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor));

    GLCall(glGenFramebuffers(1, &m_FrameBuffer));
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer));
    GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_RendererId, 0));
    GLCall(glDrawBuffer(GL_NONE));
    GLCall(glReadBuffer(GL_NONE));
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}



void DepthMap::Bind() {
    GLCall(glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT));
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer));
}

void DepthMap::BindTexture(int slot) {
    GLCall(glActiveTexture(GL_TEXTURE0 + slot));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererId));
}

