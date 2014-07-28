//
//  OGLBindableResource.h
//  OGLTest
//
//  Created by Emiel Bon on 03-02-14.
//  Copyright (c) 2014 Emiel Bon. All rights reserved.
//

#pragma once

#include "IOGLResource.h"
#include "types.h"
#include "GLError.h"

class OGLResource : public IOGLResource
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
    check_gl_error();
    GenerateFunction(1, &handle);
    check_gl_error();
}

inline GLuint OGLResource::GetHandle() const
{
    return handle;
}

inline void OGLResource::Dispose()
{
    check_gl_error();
    //DisposeFunction(1, &handle);
    check_gl_error();
}