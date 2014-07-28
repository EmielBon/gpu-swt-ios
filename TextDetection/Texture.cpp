//
//  Texture.cpp
//  OGLTest
//
//  Created by Emiel Bon on 06-02-14.
//  Copyright (c) 2014 Emiel Bon. All rights reserved.
//

#include "Texture.h"

Texture::Texture(GLuint handle, int width, int height, GLenum format, GLenum type)
    : Parameters(width, height, format, type)
{
    Setup( [&](GLsizei count, GLuint* textures) { this->handle = handle; }, glDeleteTextures, glBindTexture, GL_TEXTURE_2D);
    Generate();
}

Texture::Texture(const TextureParameters &params, const GLvoid *pixels /* = nullptr */) : Parameters(params)
{
    Setup(glGenTextures, glDeleteTextures, glBindTexture, GL_TEXTURE_2D);
    Generate();
    SetData(pixels);
}

Ptr<Texture> Texture::GetEmptyClone() const
{
    return New<Texture>(Parameters);
}

void Texture::SetData(const GLvoid* pixels)
{
    Bind();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexImage2D(GL_TEXTURE_2D, 0, Parameters.Format, GetWidth(), GetHeight(), 0, Parameters.Format, Parameters.Type, pixels);
    Unbind();
}