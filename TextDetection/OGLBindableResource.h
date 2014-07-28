//
//  OGLBindableResource.h
//  OGLTest
//
//  Created by Emiel Bon on 20-03-14.
//  Copyright (c) 2014 Emiel Bon. All rights reserved.
//

#pragma once

#include "IOGLBindableResource.h"
#include "OGLResource.h"
#include "types.h"

template<class T>
class OGLBindableResource : public OGLResource, public IOGLBindableResource
{
public:
    
    virtual ~OGLBindableResource() = default;
    
    void Setup(Function<void(GLsizei, GLuint*)> generateFunction, Function<void(GLsizei, GLuint*)> disposeFunction, Function<void(GLenum, GLuint)> bindFunction, GLenum targetName);
    
    void Bind() const;
    
    void Unbind() const;
    
    bool IsBound() const;
    
    static T* GetCurrentlyBound() { return static_cast<T*>(boundBuffer); }
    
protected:
    
    static Function<void(GLenum, GLuint)> BindFunction;
    static GLenum TargetName;
    
private:
    
    static OGLBindableResource<T> *boundBuffer;
};

template<class T> OGLBindableResource<T>* OGLBindableResource<T>::boundBuffer = nullptr;
template<class T> Function<void(GLenum, GLuint)> OGLBindableResource<T>::BindFunction;
template<class T> GLenum OGLBindableResource<T>::TargetName = GL_NONE;

template<class T>
inline void OGLBindableResource<T>::Setup(Function<void(GLsizei, GLuint*)> generateFunction, Function<void(GLsizei, GLuint*)> disposeFunction, Function<void(GLenum, GLuint)> bindFunction, GLenum targetName)
{
    BindFunction = bindFunction;
    TargetName   = targetName;
    OGLResource::Setup(generateFunction, disposeFunction);
}

template<class T>
inline void OGLBindableResource<T>::Bind() const
{
    if (!IsBound())
    {
        boundBuffer = const_cast<OGLBindableResource*>(this);
        BindFunction( TargetName, GetHandle() );
        check_gl_error();
    }
}

template<class T>
inline void OGLBindableResource<T>::Unbind() const
{
    if (IsBound())
    {
        BindFunction(TargetName, 0);
        check_gl_error();
    }
}

template<class T>
inline bool OGLBindableResource<T>::IsBound() const
{
    if (boundBuffer)
        return GetHandle() == boundBuffer->GetHandle();
    return false;
}