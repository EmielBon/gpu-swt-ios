//
//  OGLBindableResource.h
//  OGLTest
//
//  Created by Emiel Bon on 03-02-14.
//  Copyright (c) 2014 Emiel Bon. All rights reserved.
//

#pragma once

#include "types.h"
#include "GLError.h"

class OGLResource
{
public:
    
    virtual ~OGLResource() { Dispose(); };
    
    void Setup(Function<void(GLsizei, GLuint*)> generateFunction, Function<void(GLsizei, GLuint*)> disposeFunction);
    
    void Generate();
    
    GLuint GetHandle() const;
    
    void Dispose();
    
protected:
    
    GLuint handle;
    Function<void(GLsizei, GLuint*)> GenerateFunction;
    Function<void(GLsizei, GLuint*)> DisposeFunction;
};

inline void OGLResource::Setup(Function<void(GLsizei, GLuint*)> generateFunction, Function<void(GLsizei, GLuint*)> disposeFunction)
{
    GenerateFunction = generateFunction;
    DisposeFunction  = disposeFunction;
}

inline void OGLResource::Generate()
{
    GenerateFunction(1, &handle);
}

inline GLuint OGLResource::GetHandle() const
{
    return handle;
}

inline void OGLResource::Dispose()
{
    //DisposeFunction(1, &handle);
}