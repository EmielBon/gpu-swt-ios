//
//  Filter.h
//  OGLTest
//
//  Created by Emiel Bon on 22-04-14.
//  Copyright (c) 2014 Emiel Bon. All rights reserved.
//

#pragma once

#include "types.h"
#include "Program.h"

class Filter
{
protected:
    
    Filter(const String &name);
    
public:
    
    Ptr<Texture> Apply(Ptr<Texture> input);
    
    virtual void Initialize() { }
    
    void DoLoadShaderPrograms();
    
    void DoInitialize();
    
protected:
    
    virtual Ptr<Texture> PerformSteps() = 0;
    
    virtual void LoadShaderPrograms() = 0;
    
    // Reserves auxilary color buffers for use. ColorBuffer[0] is always initialized to be the current ColorAttachment0 in the frame buffer
    void ReserveColorBuffers(int count);
    
    Ptr<Program> LoadScreenSpaceProgram(const String &name);
    
    Ptr<Program> LoadProgram(const String &name);
    
    Ptr<Program> LoadProgram(const String &vertexShaderSource, const String &fragmentShaderSource);
    
    void Render(PrimitiveType primitiveType, GLenum clearOptions = GL_NONE);

    void RenderToTexture(Ptr<Texture> destination, PrimitiveType primitiveType = PrimitiveType::Triangles, GLenum clearOptions = GL_NONE);
    
    Ptr<Texture> GetColorAttachment();
    
    void SetColorAttachment(Ptr<Texture> colorAttachment);
    
    void PrintProfilingInfo() const;
    
    void PreparePerPixelVertices();
    
private:
    
    bool shadersLoaded;
    bool initialized;
    
protected:
    
    List< Ptr<Texture> > ColorBuffers;
    static Ptr<VertexBuffer> PerPixelVertices;
    
public:
    
    String Name;
    Ptr<Texture> Input;
    TimeSpan RenderTime, CompileTime, TotalTime;
};

inline Filter::Filter(const String &name)
    : shadersLoaded(false), initialized(false), Name(name), RenderTime(0), CompileTime(0), TotalTime(0)
{
    
}

inline Ptr<Program> Filter::LoadScreenSpaceProgram(const String &name)
{
    return LoadProgram("Trivial", name);
}

inline Ptr<Program> Filter::LoadProgram(const String &name)
{
    return LoadProgram(name, name);
}