//
//  Filter.cpp
//  OGLTest
//
//  Created by Emiel Bon on 22-04-14.
//  Copyright (c) 2014 Emiel Bon. All rights reserved.
//

#include "Filter.h"
#include "Program.h"
#include "Profiling.h"
#include "FrameBuffer.h"
#include "GraphicsDevice.h"
#include "Texture.h"
#include "ContentLoader.h"
#include "FrameBuffer.h"
#include "VertexPosition.h"
#include "VertexBuffer.h"
#include "SWTHelperGPU.h"

Ptr<VertexBuffer> Filter::PerPixelVertices = nullptr;

void Filter::ReserveColorBuffers(int count)
{
    ColorBuffers.clear();
    for(int i = 0; i < count; ++i)
        ColorBuffers.push_back( GetColorAttachment()->GetEmptyClone() );
}

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
        check_gl_error();
        Initialize();
        check_gl_error();
        ColorBuffers.clear();
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
    
    if (!Input)
        throw std::runtime_error(String("No input specified for filter: ") + Name);
#ifdef PROFILING
    glFinish();
    auto t = now();
#endif
    check_gl_error();
    DoLoadShaderPrograms();
    check_gl_error();
    DoInitialize();
    check_gl_error();
    auto output = PerformSteps();
    check_gl_error();
    ColorBuffers.clear();
#ifdef PROFILING
    TotalTime = now() - t;
    PrintProfilingInfo();
#endif
    return output;
}

void Filter::Render(PrimitiveType primitiveType, GLenum clearOptions /* = GL_NONE */)
{
#ifdef PROFILING
    glFinish();
    auto f = now();
#endif
    if (clearOptions != GL_NONE);
        glClear(clearOptions);
    if (!GraphicsDevice::IndexBuffer)
        GraphicsDevice::DrawArrays(primitiveType);
    else
        GraphicsDevice::DrawPrimitives(primitiveType);
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

Ptr<Texture> Filter::ApplyFilter(Filter &filter, Ptr<Texture> input)
{
    auto output = filter.Apply(input);
    RenderTime  += filter.RenderTime;
    CompileTime += filter.CompileTime;
    return output;
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
    int width  = SWTHelperGPU::InputWidth;
    int height = SWTHelperGPU::InputHeight;
    
    List<VertexPosition> vertices;
    for (int x = 0; x < width;  ++x)
    for (int y = 0; y < height; ++y)
        vertices.push_back(VertexPosition(Vector3(x, y, 0)));
    
    PerPixelVertices = New<VertexBuffer>();
    PerPixelVertices->SetData(vertices);
}
