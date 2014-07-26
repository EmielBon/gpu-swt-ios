//
//  SobelFilter.h
//  OGLTest
//
//  Created by Emiel Bon on 24-04-14.
//  Copyright (c) 2014 Emiel Bon. All rights reserved.
//

#pragma once

#include "Filter.h"

class SobelFilter : public Filter
{
private:
    
    using base = Filter;
    
public:
    
    SobelFilter();
    
    void LoadShaderPrograms();
    
    Ptr<Texture> PerformSteps();

private:
    
    void ScharrAveraging(Ptr<Texture> input, Ptr<Texture> output);
    
    void Differentiation(Ptr<Texture> input, Ptr<Texture> output);
    
private:
    
    Ptr<Program> scharr, diff;
};

inline SobelFilter::SobelFilter() : base("Sobel")
{
    
}

inline void SobelFilter::LoadShaderPrograms()
{
    scharr = LoadScreenSpaceProgram("Sobel1");
    diff   = LoadScreenSpaceProgram("Sobel2");
}