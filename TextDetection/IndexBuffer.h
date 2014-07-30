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
    
    Setup(GL_ELEMENT_ARRAY_BUFFER);
    Generate();
    
}

inline void IndexBuffer::SetData(const List<GLubyte> &indices)
{
    
    Bind();
    
    base::SetData(GL_ELEMENT_ARRAY_BUFFER, indices.size(), sizeof(GLubyte), indices.data());
    
}