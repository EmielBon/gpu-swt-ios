//
//  FrameBuffer.h
//  OGLTest
//
//  Created by Emiel Bon on 18-03-14.
//  Copyright (c) 2014 Emiel Bon. All rights reserved.
//

#pragma once

#include "IOGLBindableResource.h"
#include "RenderBufferType.h"
#include "Texture.h"
#include "types.h"

class FrameBuffer : public IOGLBindableResource<FrameBuffer>
{
public:
    
    FrameBuffer();
    
    FrameBuffer(Ptr<Texture> colorAttachment, Ptr<RenderBuffer> renderBufferAttachment = nullptr);
    
    void SetColorAttachment(Ptr<Texture> colorAttachment);
    
    void SetDepthStencil(Ptr<Texture> depthStencil);
    
    void SetDepthStencil(Ptr<RenderBuffer> depthStencil);
    
    bool IsFrameBufferComplete() const;

    // Warning! Clamps color values to [0,1]
    void CopyColorAttachment(const Texture &destination) const;
    
    template<class T>
    static List<T> ReadPixels(int x, int y, int width, int height, GLenum format, GLenum type);
    
    template<class T>
    List<T> ReadPixels(int x, int y, int width, int height);
    
    template<class T>
    T ReadPixel(int x, int y);
    
    //static List<GLubyte> ReadStencil(int x, int y, int width, int height);
    
    static List<GLfloat> ReadDepth(int x, int y, int width, int height);
    
    void Print(int x = 0, int y = 0, int width = 0, int height = 0);
    
    void Print(RenderBufferType renderBuffer, int rowCount = 0);
    
private:
    
    void AssertFrameBufferComplete() const;
    
public:
    
    // todo: refactor to better reflect OpenGL
    Ptr<Texture> ColorAttachment0;
    Ptr<Texture> DepthStencil;
    Ptr<RenderBuffer> RenderBufferAttachment;
    
    static Ptr<FrameBuffer> DefaultOffscreenFrameBuffer;
};

inline bool FrameBuffer::IsFrameBufferComplete() const
{
    return (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
}

template<class T>
inline List<T> FrameBuffer::ReadPixels(int x, int y, int width, int height, GLenum format, GLenum type)
{
    List<T> pixels(width * height);
    glReadPixels(x, y, width, height, format, type, pixels.data());
    return pixels;
}

template<class T>
inline List<T> FrameBuffer::ReadPixels(int x, int y, int width, int height)
{
    auto &params = ColorAttachment0->Parameters;
    return ReadPixels<T>(x, y, width, height, params.Format, params.Type);
}

template<class T>
inline T FrameBuffer::ReadPixel(int x, int y)
{
    return ReadPixels<T>(x, y, 1, 1)[0];
}

/*inline List<GLubyte> FrameBuffer::ReadStencil(int x, int y, int width, int height)
{
    return ReadPixels<GLubyte>(x, y, width, height, GL_STENCIL_INDEX, GL_UNSIGNED_BYTE);
}*/

inline List<GLfloat> FrameBuffer::ReadDepth(int x, int y, int width, int height)
{
    return ReadPixels<GLfloat>(x, y, width, height, GL_DEPTH_COMPONENT, GL_FLOAT);
}