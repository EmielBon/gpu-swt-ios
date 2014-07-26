//
//  CannyFilter.h
//  OGLTest
//
//  Created by Emiel Bon on 24-04-14.
//  Copyright (c) 2014 Emiel Bon. All rights reserved.
//

#pragma once

#include "Filter.h"

class CannyFilter : public Filter
{
private:
    
    using base = Filter;
    
public:
    
    CannyFilter();
    
    void LoadShaderPrograms();
    
    void Initialize();
    
    Ptr<Texture> PerformSteps();
    
private:
    
    void DetectEdges(const Texture &gradients, float lowerThreshold, float upperThreshold, Ptr<Texture> output);
    
    void ScharrAveraging(const Texture &input, Ptr<Texture> output);
    
    void Differentiation(const Texture &input, Ptr<Texture> output);
    
    void PrepareStencilTest();
    
public:
    
    Ptr<Texture> Gradients;
    
private:
    
    Ptr<Filter> gaussian;
    Ptr<Program> histogram, canny, scharr, diffCanny;
};

inline CannyFilter::CannyFilter() : base("Canny")
{
    
}