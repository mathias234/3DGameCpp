//
// Created by matha on 14/03/2018.
//

#pragma once

#include "Common.h"

class FrameBuffer {
private:
    void Create(int width, int height, int multiSampleCounts, int RenderTargetCount,  bool *borders,
                            GLenum *internalFormats, GLenum *formats, GLenum *types, GLenum *attachments);
    int m_RenderTargetCount;
    unsigned int* m_RendererId;
    unsigned int* m_IntermidiateRendererId;
    GLenum* m_InternalFormats;
    GLenum* m_Formats;
    GLenum* m_Types;
    GLenum* m_Attachments;
    int m_MultiSampleCount;

    unsigned int m_FrameBuffer;
    unsigned int m_IntermdiateFrameBuffer;

    int m_Width;
    int m_Height;

public:
    FrameBuffer(int width, int height, int multiSampleCount, int RenderTargetCount,  bool* border, GLenum* internalFormat, GLenum* format, GLenum* type, GLenum* attachment);
    FrameBuffer(int width, int height, int multiSampleCount,  bool border,  GLenum internalFormat,GLenum format, GLenum type, GLenum attachment);

    void BindAsFrameBuffer();
    void BindAsTexture(int frameBufferId, int slot);


};




