//
//  GaussianFilter.cpp
//  OGLTest
//
//  Created by Emiel Bon on 24-04-14.
//  Copyright (c) 2014 Emiel Bon. All rights reserved.
//

#include "GaussianFilter.h"
#include "Texture.h"

Ptr<Texture> GaussianFilter::PerformSteps()
{
    auto output = New<Texture>(GL_RGBA, GL_UNSIGNED_BYTE);
    auto temp = output->GetEmptyClone();
    HorizontalPass(Input, temp);
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