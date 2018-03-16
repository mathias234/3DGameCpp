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




