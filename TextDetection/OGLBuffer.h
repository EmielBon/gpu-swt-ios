//
//  OGLBuffer.h
//  OGLTest
//
//  Created by Emiel Bon on 18-03-14.
//  Copyright (c) 2014 Emiel Bon. All rights reserved.
//

#pragma once

#include "OGLBindableResource.h"
#include "types.h"

template<class T>
class OGLBuffer : public OGLBindableResource<T>
{
private:
    
    using base = OGLBindableResource<T>;
    
public:
    
    OGLBuffer();
    
    void Setup(GLenum target);
    
    void SetData(GLenum target, GLsizei count, size_t elementSize, const void *data);
    
    GLsizei Count() const { return count; }
    
private:
    
    GLsizei count;
};

template<class T>
inline OGLBuffer<T>::OGLBuffer() : count(0)
{
    
}

template<class T>
inline void OGLBuffer<T>::Setup(GLenum target)
{
    base::Setup(glGenBuffers, glDeleteBuffers, glBindBuffer, target);
}

template<class T>
inline void OGLBuffer<T>::SetData(GLenum target, GLsizei count, size_t elementSize, const void *data)
{
    
    glBufferData(target, count * elementSize, data, GL_STATIC_DRAW);
    this->count = count;
    
}