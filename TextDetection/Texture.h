//
//  Texture.h
//  OGLTest
//
//  Created by Emiel Bon on 06-02-14.
//  Copyright (c) 2014 Emiel Bon. All rights reserved.
//

#pragma once

#include "IOGLBindableResource.h"
#include "TextureParameters.h"
#include "types.h"

class Texture : public IOGLBindableResource<Texture>
{
private:
    
    using base = IOGLBindableResource<Texture>;
    
public:
    
    // Empty constructor. Setup, Generate and Initialize need to be called manually
    Texture() = default;
    
    Texture(GLuint handle, int width, int height, GLenum format, GLenum type);
    
    Texture(int width, int height, GLenum format, GLenum type, const GLvoid* pixels = nullptr);
    
    Texture(const TextureParameters &params, const GLvoid* pixels = nullptr);
    
    virtual ~Texture() = default;
    
    int GetWidth() const;
    
    int GetHeight() const;
    
    Ptr<Texture> GetEmptyClone() const;
    
    void SetData(const GLvoid* pixels);
    
public:
    
    TextureParameters Parameters;
    //static const GLenum PREFERRED_TYPE = GL_UNSIGNED_INT_8_8_8_8_REV;
    
private:
    
    static Map<Tuple<int, int, GLenum, GLenum>, Stack<GLuint>> freeTextures;
    static const int MAX_RETAINED_TEXTURES = 100;
};

inline Texture::Texture(int width, int height, GLenum format, GLenum type, const GLvoid *pixels /* = nullptr */)
    : Texture( TextureParameters(width, height, format, type), pixels )
{
    
}

inline int Texture::GetWidth() const
{
    return Parameters.Width;
}

inline int Texture::GetHeight() const
{
    return Parameters.Height;
}