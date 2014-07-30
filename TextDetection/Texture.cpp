//
//  Texture.cpp
//  OGLTest
//
//  Created by Emiel Bon on 06-02-14.
//  Copyright (c) 2014 Emiel Bon. All rights reserved.
//

#include "Texture.h"

int Texture::DefaultWidth  = 0;
int Texture::DefaultHeight = 0;
Ptr<List<GLuint>> Texture::zeros = nullptr;

Texture::Texture(int width, int height, GLenum format, GLenum type, GLvoid *pixels /* = nullptr */)
    : Width(width), Height(height), Format(format), Type(type)
{
    if (!zeros)
    {
        GLuint zero = 0;
        zeros = New<List<GLuint>>(Width * Height, zero);
    }
    
    Setup(glGenTextures, glDeleteTextures, glBindTexture, GL_TEXTURE_2D);
    Generate();
    SetData(pixels);
}

Texture::Texture(GLuint handle, int width, int height, GLenum format, GLenum type)
    : Width(width), Height(height), Format(format), Type(type)
{
    Setup( [&](GLsizei count, GLuint* textures) { this->handle = handle; }, glDeleteTextures, glBindTexture, GL_TEXTURE_2D);
    Generate();
}

Ptr<Texture> Texture::GetEmptyClone() const
{
    return New<Texture>(Width, Height, Format, Type);
}

void Texture::SetData(GLvoid* pixels)
{
    Bind();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexImage2D(GL_TEXTURE_2D, 0, Format, Width, Height, 0, Format, Type, pixels ? pixels : zeros->data());
    Unbind();
}

void Texture::SetDefaultSize(int width, int height)
{
    DefaultWidth  = width;
    DefaultHeight = height;
}