//
//  Filter.cpp
//  OGLTest
//
//  Created by Emiel Bon on 22-04-14.
//  Copyright (c) 2014 Emiel Bon. All rights reserved.
//

#include "Filter.h"
#include "GraphicsDevice.h"
#include "FrameBuffer.h"
#include "ContentLoader.h"
#include "Profiling.h"
#include "RenderWindow.h" // todo: remove
#include "VertexPosition.h"
#include "VertexBuffer.h"

Ptr<VertexBuffer> Filter::PerPixelVertices = nullptr;

void Filter::DoLoadShaderPrograms()
{
    if (shadersLoaded)
        return;
#ifdef PROFILING
    glFinish();
    auto t = now();
#endif
    LoadShaderPrograms();
#ifdef PROFILING
    glFinish();
    CompileTime += now() - t;
#endif
    shadersLoaded = true;
}

void Filter::DoInitialize()
{
    if (!initialized)
    {
        Initialize();
        /*if (!PerPixelVertices && !Input)
            throw std::runtime_error("Cannot initialize per pixel vertex buffer without input texture");
        if (!PerPixelVertices) // initialize the static per pixel vertexbuffer once
            PreparePerPixelVertices();*/
    }
    initialized = true;
}

Ptr<Texture> Filter::Apply(Ptr<Texture> input)
{
    Input = input;
    TotalTime = RenderTime = CompileTime = TimeSpan(0);
    assert(Input);
#ifdef PROFILING
    glFinish();
    auto t = now();
#endif
    DoLoadShaderPrograms();
    DoInitialize();
    auto output = PerformSteps();
#ifdef PROFILING
    TotalTime = now() - t;
    PrintProfilingInfo();
    RenderWindow::CompileTime += CompileTime;
    RenderWindow::RenderTime  += RenderTime;
#endif
    return output;
}

void Filter::Render(PrimitiveType primitiveType, GLenum clearOptions /* = GL_NONE */)
{
#ifdef PROFILING
    glFinish();
    auto f = now();
#endif
    if (clearOptions != GL_NONE)
        glClear(clearOptions);
    if (!GraphicsDevice::IndexBuffer)
        GraphicsDevice::DrawArrays(primitiveType);
    else
        GraphicsDevice::DrawElements(primitiveType);
#ifdef PROFILING
    glFinish();
    RenderTime += now() - f;
#endif
}

void Filter::RenderToTexture(Ptr<Texture> destination, PrimitiveType primitiveType /* = PrimitiveType::Triangles */, GLenum clearOptions /* = GL_NONE */)
{
    SetColorAttachment(destination);
    Render(primitiveType, clearOptions);
}

Ptr<Texture> Filter::GetColorAttachment()
{
    return FrameBuffer::DefaultOffscreenFrameBuffer->ColorAttachment0;
}

void Filter::SetColorAttachment(Ptr<Texture> colorAttachment)
{
    FrameBuffer::DefaultOffscreenFrameBuffer->SetColorAttachment(colorAttachment);
}

Ptr<Program> Filter::LoadProgram(const String &vertexShaderSource, const String &fragmentShaderSource)
{
    return ContentLoader::Load<Program>(vertexShaderSource, fragmentShaderSource);
}

void Filter::PrintProfilingInfo() const
{
    auto misc = TotalTime - RenderTime - CompileTime;
    
    printf("%s: T(%.1fms) R(%.1fms=%.1f%%) Cpl(%.1fms=%.1f%%) M(%.1fms=%.1f%%)\n",
           Name.c_str(),
           GetTimeMsec(TotalTime),
           GetTimeMsec(RenderTime),
           RenderTime * 100.0f / TotalTime,
           GetTimeMsec(CompileTime),
           CompileTime * 100.0f / TotalTime,
           GetTimeMsec(misc),
           misc * 100.0f / TotalTime);
}

void Filter::PreparePerPixelVertices()
{
    int width  = Texture::DefaultWidth;
    int height = Texture::DefaultHeight;
    
    List<VertexPosition> vertices;
    for (int x = 0; x < width;  ++x)
    for (int y = 0; y < height; ++y)
        vertices.push_back(VertexPosition(Vector3(x, y, 0)));
    
    PerPixelVertices = New<VertexBuffer>();
    PerPixelVertices->SetData(vertices);
}
