//
//  Texture.h
//  OGLTest
//
//  Created by Emiel Bon on 06-02-14.
//  Copyright (c) 2014 Emiel Bon. All rights reserved.
//

#pragma once

#include "OGLBindableResource.h"
#include "TextureParameters.h"
#include "types.h"

class Texture : public OGLBindableResource<Texture>
{
public:
    
    // Empty constructor. Setup, Generate and Initialize need to be called manually
    Texture() = default;
    
    Texture(GLuint handle, int width, int height, GLenum format, GLenum type);
    
    Texture(GLenum format, GLenum type, const GLvoid* pixels = nullptr);
    
    Texture(int width, int height, GLenum format, GLenum type, const GLvoid* pixels = nullptr);
    
    Texture(const TextureParameters &params, const GLvoid* pixels = nullptr);
    
    virtual ~Texture() = default;
    
    int GetWidth() const;
    
    int GetHeight() const;
    
    Ptr<Texture> GetEmptyClone() const;
    
    void SetData(const GLvoid* pixels);
    
    static void SetDefaultSize(int width, int height);
    
public:
    
    TextureParameters Parameters;
    static int DefaultWidth, DefaultHeight;
};

inline Texture::Texture(int width, int height, GLenum format, GLenum type, const GLvoid *pixels /* = nullptr */)
    : Texture( TextureParameters(width, height, format, type), pixels )
{
    
}

inline Texture::Texture(GLenum format, GLenum type, const GLvoid *pixels /* = nullptr */)
    : Texture( DefaultWidth, DefaultHeight, format, type, pixels )
{
    assert(DefaultWidth > 0 && DefaultHeight > 0);
}

inline int Texture::GetWidth() const
{
    return Parameters.Width;
}

inline int Texture::GetHeight() const
{
    return Parameters.Height;
}