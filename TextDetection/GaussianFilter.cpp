//
//  GaussianFilter.cpp
//  OGLTest
//
//  Created by Emiel Bon on 24-04-14.
//  Copyright (c) 2014 Emiel Bon. All rights reserved.
//

#include "GaussianFilter.h"
#include "SWTHelperGPU.h"
#include "Texture.h"

Ptr<Texture> GaussianFilter::PerformSteps()
{
    auto output = New<Texture>(SWTHelperGPU::InputWidth, SWTHelperGPU::InputHeight, GL_RGBA, GL_UNSIGNED_BYTE);
    auto temp   = New<Texture>(SWTHelperGPU::InputWidth, SWTHelperGPU::InputHeight, GL_RGBA, GL_UNSIGNED_BYTE);
    temp->Bind();
    HorizontalPass(Input, temp);
    output->Bind();
    VerticalPass(temp, output);
    return output;
}

void GaussianFilter::HorizontalPass(Ptr<Texture> input, Ptr<Texture> output)
{
    hor->Use();
    hor->Uniforms["Texture"].SetValue(*input);
    RenderToTexture(output);
}

void GaussianFilter::VerticalPass(Ptr<Texture> input, Ptr<Texture> output)
{
    ver->Use();
    ver->Uniforms["Texture"].SetValue(*input);
    RenderToTexture(output);
}