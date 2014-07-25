//
//  VertexBuffer.h
//  OGLTest
//
//  Created by Emiel Bon on 04-02-14.
//  Copyright (c) 2014 Emiel Bon. All rights reserved.
//

#pragma once

#include "OGLBuffer.h"
#include "types.h"
#include "VertexDeclaration.h"

class VertexBuffer : public OGLBuffer<VertexBuffer>
{
    friend class GraphicsDevice;
    
private:
    
    using base = OGLBuffer<VertexBuffer>;
    
public:
    
    VertexBuffer();
    
    template<class T>
    void SetData(const List<T> &data);
    
    const VertexDeclaration& GetVertexDeclaration() const;
    
private:

    VertexDeclaration vertexDeclaration;
    
public:
    
    VertexArray VertexArrayObject;
};

inline VertexBuffer::VertexBuffer()
{
    Setup(GL_ARRAY_BUFFER);
    Generate();
}

template<class T>
inline void VertexBuffer::SetData(const List<T> &data)
{
    vertexDeclaration = T::VertexDeclaration();
    Bind();
    base::SetData(GL_ARRAY_BUFFER, data.size(), sizeof(T), data.data());
    
    VertexArrayObject.Bind();
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<void*>(0));
    
    VertexArray::BindDefault();
}

inline const VertexDeclaration& VertexBuffer::GetVertexDeclaration() const
{
    return vertexDeclaration;
}