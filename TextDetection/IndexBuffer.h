//
//  IndexBuffer.h
//  OGLTest
//
//  Created by Emiel Bon on 06-02-14.
//  Copyright (c) 2014 Emiel Bon. All rights reserved.
//

#pragma once

#include "types.h"
#include "OGLBuffer.h"
#include "PrimitiveType.h"

class IndexBuffer : public OGLBuffer<IndexBuffer>
{
private:
    
    using base = OGLBuffer<IndexBuffer>;
    
public:
    
    IndexBuffer();
    
    void SetData(const List<GLubyte> &indices);
};

inline IndexBuffer::IndexBuffer()
{
    check_gl_error();
    Setup(GL_ELEMENT_ARRAY_BUFFER);
    Generate();
    check_gl_error();
}

inline void IndexBuffer::SetData(const List<GLubyte> &indices)
{
    check_gl_error();
    Bind();
    check_gl_error();
    base::SetData(GL_ELEMENT_ARRAY_BUFFER, indices.size(), sizeof(GLubyte), indices.data());
    check_gl_error();
}