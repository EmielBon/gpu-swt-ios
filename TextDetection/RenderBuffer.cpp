//
//  DepthBuffer.cpp
//  OGLTest
//
//  Created by Emiel Bon on 18-03-14.
//  Copyright (c) 2014 Emiel Bon. All rights reserved.
//

#include "RenderBuffer.h"

RenderBuffer::RenderBuffer(int width, int height, RenderBufferType type)
{
    Setup(glGenRenderbuffers, glDeleteRenderbuffers, glBindRenderbuffer, GL_RENDERBUFFER);
    Generate();
    
    BufferType = type;
    GLenum internalFormat = GL_NONE;
    
    switch(type)
    {
        case RenderBufferType::Depth:        internalFormat = GL_DEPTH_COMPONENT32_OES; break;
        case RenderBufferType::Stencil:      internalFormat = GL_STENCIL_INDEX8;        break;
        case RenderBufferType::DepthStencil: internalFormat = GL_DEPTH24_STENCIL8_OES;  break;
        default: break;
    }
    
    Bind();
        glRenderbufferStorage(GL_RENDERBUFFER, internalFormat, width, height);
    Unbind();
}