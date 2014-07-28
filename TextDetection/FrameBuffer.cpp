//
//  FrameBuffer.cpp
//  OGLTest
//
//  Created by Emiel Bon on 18-03-14.
//  Copyright (c) 2014 Emiel Bon. All rights reserved.
//

#include "FrameBuffer.h"
#include "Texture.h"
#include "GLError.h"
#include "RenderBuffer.h"

Ptr<FrameBuffer> FrameBuffer::DefaultOffscreenFrameBuffer = nullptr;

FrameBuffer::FrameBuffer()
{
    Setup(glGenFramebuffers, glDeleteFramebuffers, glBindFramebuffer, GL_FRAMEBUFFER);
    Generate();
    check_gl_error();
    // OpenGL ES only allows COLOR_ATTACHMENT0!
    //GLenum drawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
    //glDrawBuffers(1, drawBuffers);
}

FrameBuffer::FrameBuffer(Ptr<Texture> colorAttachment, Ptr<RenderBuffer> depthStencil) : FrameBuffer()
{
    SetColorAttachment(colorAttachment);
    if (depthStencil)
        SetDepthStencil(depthStencil);
}

void FrameBuffer::AssertFrameBufferComplete() const
{
    if (!IsFrameBufferComplete())
    {
        check_gl_error();
        throw std::runtime_error("Framebuffer initialization failed");
    }
}

void FrameBuffer::SetColorAttachment(Ptr<Texture> colorAttachment)
{
    if (!colorAttachment)
        throw std::runtime_error("Attempt to attach nullptr color attachment");
        
    ColorAttachment0 = colorAttachment;
    ColorAttachment0->Bind();
    // todo: remove bind
    Bind();
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ColorAttachment0->GetHandle(), 0);
    AssertFrameBufferComplete();
}

void FrameBuffer::SetDepthStencil(Ptr<Texture> depthStencil)
{
    if (!depthStencil)
        throw std::runtime_error("Attempt to attach nullptr color attachment");
    
    DepthStencil = depthStencil;
    // todo: remove bind
    Bind();
    check_gl_error();
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,   GL_TEXTURE_2D, DepthStencil->GetHandle(), 0);
    check_gl_error();
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, DepthStencil->GetHandle(), 0);
    check_gl_error();
    AssertFrameBufferComplete();
}

void FrameBuffer::SetDepthStencil(Ptr<RenderBuffer> renderBufferAttachment)
{
    if (!renderBufferAttachment)
        throw std::runtime_error("Attempt to attach nullptr renderbuffer attachment");
    
    RenderBufferAttachment = renderBufferAttachment;
    Bind();
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,   GL_RENDERBUFFER, RenderBufferAttachment->GetHandle());
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RenderBufferAttachment->GetHandle());
    AssertFrameBufferComplete();
}

/*void FrameBuffer::CopyColorAttachment(const Texture &dest) const
{
    GLenum sourceType = FrameBuffer::DefaultOffscreenFrameBuffer->ColorAttachment0->Parameters.Type;
    GLenum   destType = dest.Parameters.Type;
    
    if (sourceType == GL_HALF_FLOAT_OES)
        throw std::runtime_error("Error: Copying from a half float render target is not supported by the OES_texture_float extension");
    if (destType == GL_HALF_FLOAT_OES || destType == GL_FLOAT)
        throw std::runtime_error("Error: Copying to a (half) float texture is not supported by the OES_texture_float extension");
    
    auto prev = Texture::GetCurrentlyBound();
    dest.Bind();
    check_gl_error();
    glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, dest.GetWidth(), dest.GetHeight()); // todo: does not work with float textures in OpenGL ES 2.0 accoring to the extension
    check_gl_error();
    if (prev)
        prev->Bind();
    check_gl_error();
}*/

void FrameBuffer::Print(int x, int y, int width, int height)
{
    int w = width  == 0 ? ColorAttachment0->GetWidth()  : width;
    int h = height == 0 ? ColorAttachment0->GetHeight() : height;
    auto &params  = ColorAttachment0->Parameters;
    //GLenum type   = params.Type; // Only supports GL_UNSIGNED_BYTE for now
    GLenum format = params.Format;
    
    if (format == GL_ALPHA || format == GL_LUMINANCE || format == GL_RED_EXT)
    {
        auto pixels = ReadPixels<GLubyte>(x, y, w, h);
        for(auto pixel : pixels)
            printf("%u ", pixel);
        check_gl_error();
    }
    else if (format == GL_RG_EXT)
    {
        auto pixels = ReadPixels<Vector2b>(x, y, w, h);
        for(auto pixel : pixels)
            printf("(%u,%u)", pixel[0], pixel[1]);
        check_gl_error();
    }
    else if (format == GL_RGB)
    {
        auto pixels = ReadPixels<Vector3b>(x, y, w, h);
        for(auto pixel : pixels)
            printf("(%u,%u,%u)", pixel[0], pixel[1], pixel[2]);
        check_gl_error();
    }
    else if (format == GL_RGBA)
    {
        auto pixels = ReadPixels<Vector4b>(x, y, w, h);
        for(auto pixel : pixels)
            printf("(%u,%u,%u,%u)", pixel[0], pixel[1], pixel[2], pixel[3]);
        check_gl_error();
    }
    else
    {
        throw std::runtime_error("Error: Unsupported format for framebuffer printing");
    }
}

void FrameBuffer::Print(RenderBufferType renderBuffer, int rowCount)
{
    int width  = ColorAttachment0->GetWidth();
    int height = (rowCount == 0) ? ColorAttachment0->GetHeight() : rowCount;
    
    if (renderBuffer == RenderBufferType::Depth)
    {
        auto pixels = ReadDepth(0, 0, width, height);
        for(auto pixel : pixels)
            printf("%f ", pixel);
    }
    if (renderBuffer == RenderBufferType::Stencil)
    {
        /*auto pixels = ReadStencil(0, 0, width, height);
        for(auto pixel : pixels)
            printf("%u ", pixel);*/
        throw std::runtime_error("Error: Cannot do glReadPixels for stencil buffer in OpenGL ES 2.0");
    }
}