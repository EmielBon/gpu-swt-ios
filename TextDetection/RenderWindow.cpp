//
//  Renderer.cpp
//  OGLTest
//
//  Created by Emiel Bon on 31-01-14.
//  Copyright (c) 2014 Emiel Bon. All rights reserved.
//

#include "RenderWindow.h"

// Framework
#include "BoundingBox.h"
#include "ContentLoader.h"
#include "DrawableRect.h"
// Graphics
#include "FrameBuffer.h"
#include "GLError.h"
#include "GraphicsDevice.h"
#include "Program.h"
#include "Texture.h"
// SWT stuff
#include "Chain.h"
#include "LetterCandidate.h"
#include "SWTParameters.h"
#include "SWTHelper.h"
#include "TextRegionsFilter.h"
// Misc
#include "types.h"
#include <assert.h>
#include <stdio.h>
#include "Profiling.h"

RenderWindow* RenderWindow::instance = nullptr;
TimeSpan RenderWindow::RenderTime;
TimeSpan RenderWindow::CompileTime;

RenderWindow::RenderWindow(GLuint inputTextureHandle, GLuint width, GLuint height)
    : currentTextureIndex(0), oldTextureIndex(-1)
{
    instance = this;
    glViewport(0, 0, width, height);
    SetupGL();
    glFinish();
    auto startTime = now();
    
    auto input = New<Texture>(inputTextureHandle, width, height, GL_RGBA, GL_UNSIGNED_BYTE); // These parameters probably have no impact, except if you want an empty copy of this texture
    Texture::SetDefaultSize(width, height);
    
    AddTexture(input, "Input image");
    
    rect1 = New<DrawableRect>(-1, -1, 1, 1);
    GraphicsDevice::SetDefaultBuffers(rect1->VertexBuffer, rect1->IndexBuffer);
    GraphicsDevice::UseDefaultBuffers();
    
    program = Program::LoadScreenSpaceProgram("Normal");
    
    auto letterCandidates = PerformStrokeWidthTransformGPU(input);
    
    /*auto letters = SWTHelper::FilterOnOverlappingBoundingBoxes(letterCandidates);
    //DrawBoundingBoxes(input, components, "Boundingboxes with overlap <= 2 (letters)");
    auto chains = SWTHelper::MakePairs(letters);
    //DrawChains(input, chains, "Paired components");
    
    while(SWTHelper::ChainingIteration(chains))
        ;
    //DrawChains(input, chains, "Chains");
    
    SWTHelper::FilterNonWords(chains);
    //DrawChains(input, chains, "Chains with length >= 3 (words)");
    
    List<BoundingBox> boundingBoxes;
    
    for(auto chain : chains)
        boundingBoxes.push_back(chain->BoundingBox());
    
    //cv::Mat output = ImgProc::DrawBoundingBoxes(input, boundingBoxes, {0, 255, 255, 255});
    //AddTexture(output, "Detected text regions");
    */
    glFinish();
    auto totalTime = now() - startTime;
    auto misc = totalTime - RenderTime - CompileTime;
    
    printf("\n");
    printf("Setup time: %.1fms (%.1f%%)\n",  GetTimeMsec(CompileTime), CompileTime * 100.0f / totalTime);
    printf("Render time: %.1fms (%.1f%%)\n", GetTimeMsec(RenderTime),  RenderTime  * 100.0f / totalTime);
    printf("Misc time: %.1fms (%.1f%%)\n",   GetTimeMsec(misc),        misc        * 100.0f / totalTime);
    printf("Total time: %.1fms\n",           GetTimeMsec(totalTime));
}

void RenderWindow::SetupGL()
{
    DisableIrrelvantState();
    // Create and setup framebuffer
    FrameBuffer::DefaultOffscreenFrameBuffer = New<FrameBuffer>();
    // Create the framebuffer attachments
    //auto depthStencil = New<Texture>(width, height, GL_DEPTH_STENCIL_OES, GL_UNSIGNED_INT_24_8_OES);
    //FrameBuffer::DefaultOffscreenFrameBuffer->SetDepthStencil(depthStencil);
}

void RenderWindow::DrawTexture(int index)
{
    assert(index >= 0);
    
    glViewport(0, 0, Texture::DefaultWidth, Texture::DefaultHeight);
    glClearColor(0, 1, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    auto texture = textures[index % textures.size()];
    program->Use();
    program->Uniforms["Texture"].SetValue(*texture);
    if (!GraphicsDevice::IndexBuffer)
        GraphicsDevice::DrawArrays(PrimitiveType::Triangles);
    else
        GraphicsDevice::DrawPrimitives(PrimitiveType::Triangles);
}

void RenderWindow::DisableIrrelvantState()
{
    glDisable(GL_CULL_FACE);
    glDisable(GL_DITHER);
    glDisable(GL_BLEND);
    glDisable(GL_STENCIL_TEST);
    glDisable(GL_DEPTH_TEST);
    // todo: more?
}

const char* RenderWindow::GetTextureName(int index)
{
    return textureDescriptors[index % textures.size()].c_str();
}

void RenderWindow::AddTexture(Ptr<Texture> input, const String &descriptor /* = "" */)
{
    if (!input)
        return;
    textures.push_back(input);
    textureDescriptors.push_back(descriptor);
}

List<Ptr<LetterCandidate>> RenderWindow::PerformStrokeWidthTransformGPU(Ptr<Texture> input)
{
    auto textRegionsFilter = New<TextRegionsFilter>();
    textRegionsFilter->Apply(input);
    return textRegionsFilter->LetterCandidates;
}

/*void DrawChains(const cv::Mat &input, const LinkedList< Ptr<Chain> > &chains, const String &description)
{
    cv::Mat lineImage = input.clone();
    for(auto chain : chains)
    {
        for(auto it = chain->LetterCandidates.begin(); it != chain->LetterCandidates.end(); ++it)
        {
            auto c1 = *it;
            
            cv::rectangle(lineImage, c1->BoundingBox.Bounds, {0, 255, 255, 255});
            
            if (std::next(it) == chain->LetterCandidates.end())
                break;
            
            auto c2 = *std::next(it);
            
            cv::line(lineImage, c1->BoundingBox.Center(), c2->BoundingBox.Center(), {0, 255, 0, 255}, 1);
        }
    }
    RenderWindow::Instance().AddTexture(lineImage, description);
}

void RenderWindow::SetWindowSize(const cv::Size &size, const cv::Size &max)
{
    float aspectRatio = size.width / (float)size.height;
    int width = std::min(size.width, max.width);
    //int height = std::min(size.height, max.height);
    
    glfwSetWindowSize(window, width, width / aspectRatio);
}
*/