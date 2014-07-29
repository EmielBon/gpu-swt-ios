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
    glFinish();
    auto output = New<Texture>(GL_RGBA, GL_UNSIGNED_BYTE); glFinish();
    auto temp = output->GetEmptyClone(); glFinish();
    HorizontalPass(Input, temp); glFinish();
    VerticalPass(temp, output); glFinish();
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