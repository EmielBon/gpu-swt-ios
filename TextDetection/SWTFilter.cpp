//
//  SWTFilter.cpp
//  OGLTest
//
//  Created by Emiel Bon on 25-04-14.
//  Copyright (c) 2014 Emiel Bon. All rights reserved.
//

#include "SWTFilter.h"
#include "VertexPosition.h"
#include "Texture.h"
#include "GraphicsDevice.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "SobelFilter.h"
#include "CannyFilter.h"
#include "RenderWindow.h"
#include "GaussianFilter.h"
#include "FrameBuffer.h"

void SWTFilter::LoadShaderPrograms()
{
    sobel = New<SobelFilter>();
    canny = New<CannyFilter>();
    
    sobel->DoLoadShaderPrograms();
    canny->DoLoadShaderPrograms();
    
    /*cast     = LoadScreenSpaceProgram("CastRays");
    write    = LoadProgram("WriteRays", "Value");
    avg      = LoadScreenSpaceProgram("AverageRays");
    writeAvg = LoadProgram("WriteAverageRays", "Value");*/
}

void SWTFilter::Initialize()
{
    gradients = sobel->Apply(Input);
    DEBUG_FB(gradients, "Gradients");
    edges     = canny->Apply(Input);
    DEBUG_FB(edges, "Edges");
    //PrepareEdgeOnlyStencil();
    //PrepareRayLines(*edges);
    
    glClearColor(0, 0, 0, 0);
}

void SWTFilter::PrepareEdgeOnlyStencil()
{
    glClearStencil(0);
    glStencilFunc(GL_EQUAL, 2, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
}

void SWTFilter::PrepareRayLines(const Texture &input)
{
    /*int width  = input.GetWidth();
    int height = input.GetHeight();
    
    GLfloat buffer[width * height];
    input.GetTextureImage(GL_RED, GL_FLOAT, buffer);
    
    int count = 0;
    for(int i = 0; i < width;  ++i)
    for(int j = 0; j < height; ++j)
    {
        if (buffer[i + j * width] != 0.0f)
            count++;
    }
    
    List<VertexPosition> vertices;
    vertices.reserve(count);
    
    for(int i = 0; i < width;  ++i)
    for(int j = 0; j < height; ++j)
    {
        if (buffer[i + j * width] == 0.0f)
            continue;
        
        VertexPosition v1, v2;
        v1.Position = Vector3(i, j, 0); // z == 0 = Use directly
        v2.Position = Vector3(i, j, 1); // z == 1 = Scatter position to end point
        vertices.push_back(v1);
        vertices.push_back(v2);
    }
    
    linesVertices = New<VertexBuffer>();
    linesVertices->SetData(vertices);*/
}

Ptr<Texture> SWTFilter::PerformSteps()
{
    /*if (GradientDirection == GradientDirection::Unspecified)
        throw std::runtime_error("Unspecified gradient direction in SWT step");
    
    bool darkOnLight = (GradientDirection == GradientDirection::With);
    
    ReserveColorBuffers(2);
    
    auto swt               = output;
    auto oppositePositions = ColorBuffers[0];
    auto averageValues     = ColorBuffers[1];
    
    //glEnable(GL_STENCIL_TEST);
    GraphicsDevice::UseDefaultBuffers();
    PrepareEdgeOnlyStencil();
    glDisable(GL_STENCIL_TEST);
    glDisable(GL_DEPTH_TEST);
    CastRays(darkOnLight, oppositePositions);
    //glDisable(GL_STENCIL_TEST);
    DEBUG_FB("SWT 1");
    GraphicsDevice::SetBuffers(linesVertices, nullptr);
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE);
    glBlendEquation(GL_MAX);
    WriteRayValues(*oppositePositions, swt);
    WriteRayValues2(*oppositePositions, swt);
    glDisable(GL_BLEND);
    GraphicsDevice::UseDefaultBuffers();
    DEBUG_FB("SWT 2");
    //glEnable(GL_STENCIL_TEST);
    AverageRayValues(*oppositePositions, *swt, averageValues);
    //glDisable(GL_STENCIL_TEST);
    DEBUG_FB("SWT 3");
    GraphicsDevice::SetBuffers(linesVertices, nullptr);
    glEnable(GL_BLEND);
    WriteAverageRayValues(*oppositePositions, *averageValues, output);
    WriteAverageRayValues2(*oppositePositions, *averageValues, output);
    glDisable(GL_BLEND);
    DEBUG_FB("SWT 4");
    
    GraphicsDevice::UseDefaultBuffers();*/
    return nullptr;
}

void SWTFilter::CastRays(bool darkOnLight, Ptr<Texture> output)
{
    cast->Use();
    cast->Uniforms["Edges"].SetValue(*edges);
    cast->Uniforms["Gradients"].SetValue(*gradients);
    cast->Uniforms["DarkOnLight"].SetValue(darkOnLight);
    RenderToTexture(output, PrimitiveType::Unspecified, GL_COLOR_BUFFER_BIT);
}

void SWTFilter::WriteRayValues(const Texture &oppositePositions, Ptr<Texture> output)
{
    write->Use();
    write->Uniforms["OppositePositions"].SetValue(oppositePositions);
    RenderToTexture(output, PrimitiveType::Lines, GL_COLOR_BUFFER_BIT);
}

void SWTFilter::WriteRayValues2(const Texture &oppositePositions, Ptr<Texture> output)
{
    write->Use();
    write->Uniforms["OppositePositions"].SetValue(oppositePositions);
    RenderToTexture(output, PrimitiveType::Points);
}

void SWTFilter::AverageRayValues(const Texture &oppositePositions, const Texture &values, Ptr<Texture> output)
{
    avg->Use();
    avg->Uniforms["OppositePositions"].SetValue(oppositePositions);
    avg->Uniforms["Values"].SetValue(values);
    RenderToTexture(output, PrimitiveType::Unspecified, GL_COLOR_BUFFER_BIT);
}

void SWTFilter::WriteAverageRayValues(const Texture &oppositePositions, const Texture &averageValues, Ptr<Texture> output)
{
    writeAvg->Use();
    writeAvg->Uniforms["OppositePositions"].SetValue(oppositePositions);
    writeAvg->Uniforms["AverageValues"].SetValue(averageValues);
    RenderToTexture(output, PrimitiveType::Lines, GL_COLOR_BUFFER_BIT);
}

void SWTFilter::WriteAverageRayValues2(const Texture &oppositePositions, const Texture &averageValues, Ptr<Texture> output)
{
    writeAvg->Use();
    writeAvg->Uniforms["OppositePositions"].SetValue(oppositePositions);
    writeAvg->Uniforms["AverageValues"].SetValue(averageValues);
    RenderToTexture(output, PrimitiveType::Points);
}