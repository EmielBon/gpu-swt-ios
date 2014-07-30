//
//  TextRegionsFilter.cpp
//  OGLTest
//
//  Created by Emiel Bon on 23-05-14.
//  Copyright (c) 2014 Emiel Bon. All rights reserved.
//

#include "TextRegionsFilter.h"
#include "GrayFilter.h"
#include "SWTFilter.h"
//#include "ConnectedComponentsFilter.h"
#include "Texture.h"
#include "FrameBuffer.h"
#include "GaussianFilter.h"
#include "VertexPosition.h"
#include "VertexBuffer.h"
#include "GraphicsDevice.h"
#include "RenderWindow.h"
#include "SWTHelperGPU.h"
#include "SWTParameters.h"
#include "LetterCandidate.h"
#include <cmath>

void TextRegionsFilter::LoadShaderPrograms()
{
    grayFilter                = New<GrayFilter>();
    swtFilter                 = New<SWTFilter>();
    //connectedComponentsFilter = New<ConnectedComponentsFilter>();
    
    grayFilter->DoLoadShaderPrograms();
    swtFilter->DoLoadShaderPrograms();
    //connectedComponentsFilter->DoLoadShaderPrograms();
    
    /*boundingBoxes             = LoadProgram("ScatterToRoot", "BoundingBoxes");
    filterInvalidComponents   = LoadScreenSpaceProgram("FilterInvalidComponents");
    filterComponentProperties = LoadScreenSpaceProgram("FilterComponentProperties");
    countComponents           = LoadProgram("CountComponents");
    stencilRouting            = LoadProgram("StencilRouting");
    countPixels               = LoadProgram("ScatterToRoot", "Increment");
    average                   = LoadProgram("ScatterToRoot", "AverageColorAndSWT");
    variance                  = LoadProgram("ScatterToRoot", "Variance");*/
    //writeIDs              = LoadScreenSpaceProgram("WriteIDs");
    //vertexTexture = LoadProgram("VertexTexture");
}

void TextRegionsFilter::Initialize()
{
    
    gray = grayFilter->Apply(Input); 
    DEBUG_FB(gray, "Gray");
    //PreparePerPixelVertices();
}

void TextRegionsFilter::PrepareBoundingBoxCalculation()
{
    glBlendEquation(GL_MAX_EXT);
    glBlendFunc(GL_ONE, GL_ONE);
}

void TextRegionsFilter::PrepareSummationOperations()
{
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_ONE, GL_ONE);
}

Ptr<Texture> TextRegionsFilter::PerformSteps()
{
    // Attention: output is null;
    
    //ReserveColorBuffers(12);
    
    /*auto swt1        = ColorBuffers[0];
     auto swt2        = ColorBuffers[1];
     auto components1 = ColorBuffers[2];
     auto components2 = ColorBuffers[3];
     auto bboxes      = ColorBuffers[4];
     auto filtered    = ColorBuffers[5];
     auto pixelCount  = ColorBuffers[6];
     auto averages    = ColorBuffers[7];
     auto varianceSum = ColorBuffers[8];
     auto temp        = ColorBuffers[9];
     auto filteredAverages = ColorBuffers[10];
     auto output2     = ColorBuffers[11];
     */
    // Calculate SWT
    swtFilter->GradientDirection = GradientDirection::With;
    auto swt1 = swtFilter->Apply(gray);
    /*
     swtFilter->GradientDirection = GradientDirection::Against;
     ApplyFilter(*swtFilter, swt2);
     
     // Determine connected components
     connectedComponentsFilter->Input = swt1;
     ApplyFilter(*connectedComponentsFilter, components1);
     connectedComponentsFilter->Input = swt2;
     ApplyFilter(*connectedComponentsFilter, components2);
     
     // Prepare summations
     PrepareSummationOperations();
     GraphicsDevice::SetBuffers(PerPixelVertices, nullptr);
     glEnable(GL_BLEND);
     
     // Calculate pixel count in components
     CountPixels(components1, pixelCount, true);
     CountPixels(components2, pixelCount, false);
     DEBUG_FB("Component pixel counts");
     
     // Average component color and SWT
     AverageColorAndSWT(components1, pixelCount, Input, swt1, averages, true);
     AverageColorAndSWT(components2, pixelCount, Input, swt2, averages, false);
     */
    /*auto pixels = FrameBuffer::DefaultOffscreenFrameBuffer->ReadPixels<cv::Vec4f>(0, 0, 800, 600, GL_RGBA, GL_FLOAT);
     for(auto& pixel : pixels)
     {
     if (pixel[3] != 0.0)
     printf("%f ", pixel[3]);
     }*/
    /*
     DEBUG_FB("Average component colors");
     
     // Calculate variance
     Variance(components1, pixelCount, swt1, averages, varianceSum, true);
     Variance(components2, pixelCount, swt2, averages, varianceSum, false);
     DEBUG_FB("Component variances");
     
     // Compute bounding boxes
     PrepareBoundingBoxCalculation();
     BoundingBoxes(components1, bboxes, true);
     BoundingBoxes(components2, bboxes, false);
     DEBUG_FB("Bounding Boxes");
     
     // End summations
     GraphicsDevice::UseDefaultBuffers();
     glDisable(GL_BLEND);
     
     // Filter invalid components
     FilterInvalidComponents(bboxes, averages, pixelCount, varianceSum, filtered);
     DEBUG_FB("Valids");
     FilterComponentProperties(filtered, averages, filteredAverages);
     
     // Count unique components
     GraphicsDevice::SetBuffers(PerPixelVertices, nullptr);
     PrepareSummationOperations();
     glEnable(GL_BLEND);
     CountComponents(filtered, temp);
     int componentCount = (int)FrameBuffer::ReadPixel(1, 1)[0];
     int N = (int)ceil(sqrt(componentCount));
     printf("Number of unique components: %i\n", componentCount);
     if (componentCount > 255)
     printf("Warning: Component count > 255\n");
     glDisable(GL_BLEND);
     
     // Stencil routing
     PrepareStencilRouting(N);
     glEnable(GL_STENCIL_TEST);
     StencilRouting(filtered, N, output);
     PrepareStencilRouting(N);
     StencilRouting(filteredAverages, N, output2);
     ExtractLetterCandidates(output, output2, N, componentCount);
     glDisable(GL_STENCIL_TEST);
     glPointSize(1);
     
     GraphicsDevice::UseDefaultBuffers();*/
    return nullptr;
}

void TextRegionsFilter::FilterInvalidComponents(Ptr<Texture> boundingBoxes, Ptr<Texture> averages, Ptr<Texture> pixelCounts, Ptr<Texture> varianceSums, Ptr<Texture> output)
{
    filterInvalidComponents->Use();
    filterInvalidComponents->Uniforms["BoundingBoxes"].SetValue(*boundingBoxes);
    filterInvalidComponents->Uniforms["Averages"].SetValue(*averages);
    filterInvalidComponents->Uniforms["PixelCounts"].SetValue(*pixelCounts);
    filterInvalidComponents->Uniforms["VarianceSums"].SetValue(*varianceSums);
    //RenderToTexture(output);
}

void TextRegionsFilter::FilterComponentProperties(Ptr<Texture> filteredBoundingBoxes, Ptr<Texture> averages, Ptr<Texture> output)
{
    filterComponentProperties->Use();
    filterComponentProperties->Uniforms["BoundingBoxes"].SetValue(*filteredBoundingBoxes);
    filterComponentProperties->Uniforms["Averages"].SetValue(*averages);
    //RenderToTexture(output);
}

void TextRegionsFilter::BoundingBoxes(Ptr<Texture> components, Ptr<Texture> output, bool clear)
{
    boundingBoxes->Use();
    boundingBoxes->Uniforms["Components"].SetValue(*components);
    RenderToTexture(output, PrimitiveType::Points, clear ? GL_COLOR_BUFFER_BIT : 0);
}

void TextRegionsFilter::CountComponents(Ptr<Texture> input, Ptr<Texture> output)
{
    countComponents->Use();
    countComponents->Uniforms["Texture"].SetValue(*input);
    RenderToTexture(output, PrimitiveType::Points, GL_COLOR_BUFFER_BIT);
}

void TextRegionsFilter::CountPixels(Ptr<Texture> components, Ptr<Texture> output, bool clear)
{
    countPixels->Use();
    countPixels->Uniforms["Components"].SetValue(*components);
    RenderToTexture(output, PrimitiveType::Points, clear ? GL_COLOR_BUFFER_BIT : 0);
}

void TextRegionsFilter::AverageColorAndSWT(Ptr<Texture> components, Ptr<Texture> pixelCounts, Ptr<Texture> inputImage, Ptr<Texture> swt, Ptr<Texture> output, bool clear)
{
    average->Use();
    average->Uniforms["Components"].SetValue(*components);
    average->Uniforms["PixelCounts"].SetValue(*pixelCounts);
    average->Uniforms["InputImage"].SetValue(*inputImage);
    average->Uniforms["StrokeWidths"].SetValue(*swt);
    RenderToTexture(output, PrimitiveType::Points, clear ? GL_COLOR_BUFFER_BIT : 0);
}

void TextRegionsFilter::Variance(Ptr<Texture> components, Ptr<Texture> pixelCounts, Ptr<Texture> strokeWidths, Ptr<Texture> averages, Ptr<Texture> output, bool clear)
{
    variance->Use();
    variance->Uniforms["Components"].SetValue(*components);
    variance->Uniforms["PixelCounts"].SetValue(*pixelCounts);
    variance->Uniforms["StrokeWidths"].SetValue(*strokeWidths);
    variance->Uniforms["Averages"].SetValue(*averages);
    RenderToTexture(output, PrimitiveType::Points, clear ? GL_COLOR_BUFFER_BIT : 0);
}

void TextRegionsFilter::PrepareStencilRouting(int N)
{
    int width  = Input->Width;
    int height = Input->Height;
    
    List<GLuint> pixels(N * N, 1);
    for(int i = 1; i < N * N; ++i)
        pixels[i] = pixels[i-1] + 1;
    
    stencil = New<Texture>(width, height, GL_DEPTH_STENCIL_OES, GL_UNSIGNED_INT_24_8_OES);
    FrameBuffer::DefaultOffscreenFrameBuffer->SetDepthStencil(stencil);
    
    glClearStencil(0);
    glClear(GL_STENCIL_BUFFER_BIT);
    glStencilFunc(GL_EQUAL, 1, 0xFF);
    glStencilOp(GL_DECR, GL_DECR, GL_DECR);
    
    stencil->Bind();
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, N, N, GL_DEPTH_STENCIL_OES, GL_UNSIGNED_INT_24_8_OES, pixels.data());
    
    // todo: glPointSize is now a built-in special variable for vertex shaders (gl_PointSize)
    //glPointSize(N + 1); // todo: why is +1 needed? Or maybe it just needs to be an even number, in the test case it was 7 and the first column of pixels was not drawn
}

void TextRegionsFilter::ExtractLetterCandidates(Ptr<Texture> filteredBoundingBoxes, Ptr<Texture> filteredAverages, int N, int count)
{
    SetColorAttachment(filteredBoundingBoxes);
    auto bboxPixels = FrameBuffer::DefaultOffscreenFrameBuffer->ReadPixels<cv::Vec4f>(0, 0, N, N, GL_RGBA, GL_FLOAT);
    SetColorAttachment(filteredAverages);
    auto averagesPixels = FrameBuffer::DefaultOffscreenFrameBuffer->ReadPixels<cv::Vec4f>(0, 0, N, N, GL_RGBA, GL_FLOAT);
    
    for(int i = 0; i < (int)bboxPixels.size(); ++i)
    {
        auto bbox    = bboxPixels[i];
        auto average = averagesPixels[i];
        
        auto letter = New<LetterCandidate>();
        
        // Retrieve bounding box
        int x1 = -((int)bbox[0] - (Input->Width - 1));
        int y1 = -((int)bbox[1] - (Input->Height - 1));
        int x2 = (int)bbox[2];
        int y2 = (int)bbox[3];
        letter->BoundingBox = BoundingBox(cv::Rect(x1, y1, x2 - x1, y2 - y1));
        
        letter->MeanColor = cv::Vec3b((uchar)(average[0] * 255), (uchar)(average[1] * 255), (uchar)(average[2] * 255));
        letter->MeanStrokeWidth = average[3];
        if (x2 - x1 >= 0 && y2 - y1 >= 0)
            LetterCandidates.push_back(letter);
    }
    //if (ExtractedBoundingBoxes.size() != count)
    //    throw std::runtime_error("Error: Missing some bounding boxes");
}

void TextRegionsFilter::StencilRouting(Ptr<Texture> input, float N, Ptr<Texture> output)
{
    stencilRouting->Use();
    stencilRouting->Uniforms["Texture"].SetValue(*input);
    stencilRouting->Uniforms["StencilCenter"].SetValue(Vector2(N / 2, N / 2));
    RenderToTexture(output, PrimitiveType::Points, GL_COLOR_BUFFER_BIT);
}

/*void TextRegionsFilter::WriteIDs(Ptr<Texture> input, Ptr<Texture> output)
{
    writeIDs->Use();
    writeIDs->Uniforms["Texture"].SetValue(*input);
    RenderToTexture(output);
}*/