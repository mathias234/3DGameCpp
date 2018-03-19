//
// Created by matha on 14/03/2018.
//

#pragma once

#include "Common.h"

class DepthMap {
private:
    const unsigned int SHADOW_WIDTH = 4096, SHADOW_HEIGHT = 4096;
    unsigned int m_RendererId;
    unsigned int m_FrameBuffer;

public:
    DepthMap();
    void Bind();

    void BindTexture(int slot);
};




