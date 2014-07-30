//
//  SWTHelperGPU.cpp
//  OGLTest
//
//  Created by Emiel Bon on 25-03-14.
//  Copyright (c) 2014 Emiel Bon. All rights reserved.
//

#include "SWTHelperGPU.h"
#include "GraphicsDevice.h"
#include "FrameBuffer.h"
#include "ContentLoader.h"
#include "Texture.h"
#include "DrawableRect.h"
#include "RenderWindow.h"
#include "BoundingBox.h"
#include "Profiling.h"
//#include "TextureUtil.h"
#include "TextRegionsFilter.h"
#include "VertexPosition.h"
#include "RenderBuffer.h"
#include "RenderBufferType.h"

TimeSpan renderTime, copyTime, compileTime;

List< Ptr<LetterCandidate> > SWTHelperGPU::StrokeWidthTransform(Ptr<Texture> input)
{
    glViewport(0, 0, Texture::DefaultWidth, Texture::DefaultHeight);
    glFinish();
    auto startTime = now();
    
    DisableIrrelvantState();
    // Create the framebuffer attachments
    //auto depthStencil = New<Texture>(width, height, GL_DEPTH_STENCIL_OES, GL_UNSIGNED_INT_24_8_OES);
    // Create and setup framebuffer
    FrameBuffer::DefaultOffscreenFrameBuffer = New<FrameBuffer>();
    //frameBuffer.SetDepthStencil(depthStencil);
    
    auto textRegionsFilter = New<TextRegionsFilter>();
    
    textRegionsFilter->DoLoadShaderPrograms();
    
    glFinish();
    auto setupTime = now() - startTime;
    
    textRegionsFilter->Apply(input);
    
    renderTime  += textRegionsFilter->RenderTime;
    compileTime += textRegionsFilter->CompileTime;
    
    //FrameBuffer::DefaultOffscreenFrameBuffer->Print(RenderBufferType::Stencil);
    
    glFinish();
    auto totalTime = now() - startTime;
    
    auto misc = totalTime - renderTime - setupTime;
    
    printf("\n");
    printf("Total time: %.1fms\n",           GetTimeMsec(totalTime));
    printf("Setup time: %.1fms (%.1f%%)\n",  GetTimeMsec(setupTime),  setupTime  * 100.0f / totalTime);
    printf("Render time: %.1fms (%.1f%%)\n", GetTimeMsec(renderTime), renderTime * 100.0f / totalTime);
    printf("Misc time: %.1fms (%.1f%%)\n",   GetTimeMsec(misc),       misc       * 100.0f / totalTime);
    
    return textRegionsFilter->LetterCandidates;
}

void SWTHelperGPU::DisableIrrelvantState()
{
    glDisable(GL_CULL_FACE); 
    glDisable(GL_DITHER); 
    glDisable(GL_BLEND); 
    glDisable(GL_STENCIL_TEST); 
    glDisable(GL_DEPTH_TEST); 
    // todo: more?
}