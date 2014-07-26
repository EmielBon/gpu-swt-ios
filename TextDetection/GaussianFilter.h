//
//  GaussianFilter.h
//  OGLTest
//
//  Created by Emiel Bon on 24-04-14.
//  Copyright (c) 2014 Emiel Bon. All rights reserved.
//

#pragma once

#include "Filter.h"

class GaussianFilter : public Filter
{
private:
    
    using base = Filter;
    
public:
    
    GaussianFilter();
    
    void LoadShaderPrograms();
    
    Ptr<Texture> PerformSteps();
    
private:
    
    void HorizontalPass(Ptr<Texture> input, Ptr<Texture> output);
    
    void VerticalPass(Ptr<Texture> input, Ptr<Texture> output);
    
private:
    
    Ptr<Program> hor, ver;
};

inline GaussianFilter::GaussianFilter() : base("Gaussian Blur")
{
    
}

inline void GaussianFilter::LoadShaderPrograms()
{
    hor = LoadScreenSpaceProgram("GaussianBlurH");
    ver = LoadScreenSpaceProgram("GaussianBlurV");
}