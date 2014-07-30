//
//  Texture.h
//  OGLTest
//
//  Created by Emiel Bon on 06-02-14.
//  Copyright (c) 2014 Emiel Bon. All rights reserved.
//

#pragma once

#include "OGLBindableResource.h"
#include "types.h"

class Texture : public OGLBindableResource<Texture>
{
public:
    
    // Empty constructor. Setup, Generate and Initialize need to be called manually
    Texture() = default;
    
    Texture(GLuint handle, int width, int height, GLenum format, GLenum type);
    
    Texture(GLenum format, GLenum type, GLvoid* pixels = nullptr);
    
    Texture(int width, int height, GLenum format, GLenum type, GLvoid* pixels = nullptr);
    
    virtual ~Texture() = default;
    
    Ptr<Texture> GetEmptyClone() const;
    
    void SetData(GLvoid* pixels);
    
    static void SetDefaultSize(int width, int height);
    
public:
    
    int Width, Height;
    GLenum Format, Type;
    static int DefaultWidth, DefaultHeight;
    static Ptr<List<GLuint>> zeros;
};

inline Texture::Texture(GLenum format, GLenum type, GLvoid *pixels /* = nullptr */)
    : Texture( DefaultWidth, DefaultHeight, format, type, pixels )
{
    assert(DefaultWidth > 0 && DefaultHeight > 0);
}