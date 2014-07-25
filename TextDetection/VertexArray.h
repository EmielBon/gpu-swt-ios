//
//  VertexArray.h
//  OGLTest
//
//  Created by Emiel Bon on 04-02-14.
//  Copyright (c) 2014 Emiel Bon. All rights reserved.
//

#pragma once

#include "IOGLResource.h"
#include "types.h"

class VertexArray : public IOGLResource
{
public:
    
    VertexArray();
    
    void Bind();
    
    static void BindDefault();
    
    void Unbind();
    
public:
    
    GLuint handle;
};

inline VertexArray::VertexArray()
{
    Setup(glGenVertexArraysOES, glDeleteVertexArraysOES);
    Generate();
}

inline void VertexArray::Bind()
{
    glBindVertexArrayOES( GetHandle() );
}

inline void VertexArray::BindDefault()
{
    glBindVertexArrayOES(0);
}

inline void VertexArray::Unbind()
{
    BindDefault();
}