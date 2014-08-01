//
//  SobelFilter.cpp
//  OGLTest
//
//  Created by Emiel Bon on 24-04-14.
//  Copyright (c) 2014 Emiel Bon. All rights reserved.
//

#include "SobelFilter.h"
#include "Texture.h"

Ptr<Texture> SobelFilter::PerformSteps()
{
    auto output = New<Texture>(GL_RG_EXT, GL_HALF_FLOAT_OES);
    auto temp   = output->GetEmptyClone();
    ScharrAveraging(Input, temp);
    Differentiation(temp, output);
    return output;
}

void SobelFilter::ScharrAveraging(Ptr<Texture> input, Ptr<Texture> output)
{
    scharr->Use();
    scharr->Uniforms["Texture"].SetValue(*input);
    RenderToTexture(output);
}

void SobelFilter::Differentiation(Ptr<Texture> input, Ptr<Texture> output)
{
    diff->Use();
    diff->Uniforms["Texture"].SetValue(*input);
    RenderToTexture(output);
}