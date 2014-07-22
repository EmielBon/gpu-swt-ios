//
//  TextRegionsFilter.h
//  OGLTest
//
//  Created by Emiel Bon on 23-05-14.
//  Copyright (c) 2014 Emiel Bon. All rights reserved.
//

#pragma once

#include "Filter.h"

class TextRegionsFilter : public Filter
{
private:
    
    using base = Filter;
    
public:
    
    TextRegionsFilter(Ptr<Texture> input = nullptr);
    
    void LoadShaderPrograms();
    
    void Initialize();
    
    void PrepareBoundingBoxCalculation();
    
    void PrepareSummationOperations();
    
    void FilterInvalidComponents(Ptr<Texture> boundingBoxes, Ptr<Texture> averages, Ptr<Texture> pixelCounts, Ptr<Texture> variances, Ptr<Texture> output);
    
    void FilterComponentProperties(Ptr<Texture> filteredBoundingBoxes, Ptr<Texture> averages, Ptr<Texture> output);
    
    void BoundingBoxes(Ptr<Texture> input, Ptr<Texture> output, bool clear);
    
    void CountComponents(Ptr<Texture> input, Ptr<Texture> output);
    
    void CountPixels(Ptr<Texture> input, Ptr<Texture> output, bool clear);
    
    void AverageColorAndSWT(Ptr<Texture> components, Ptr<Texture> pixelCounts, Ptr<Texture> inputImage, Ptr<Texture> strokeWidths, Ptr<Texture> output, bool clear);
    
    void Variance(Ptr<Texture> components, Ptr<Texture> pixelCounts, Ptr<Texture> strokeWidths, Ptr<Texture> averages, Ptr<Texture> output, bool clear);
    
    void PrepareStencilRouting(int N);
    
    void StencilRouting(Ptr<Texture> input, float N, Ptr<Texture> output);
    
    //void WriteIDs(Ptr<Texture> input, Ptr<Texture> output);
    
    void ExtractLetterCandidates(Ptr<Texture> filteredBoundingBoxes, Ptr<Texture> filteredAverages, int N, int count);
    
    void PerformSteps(Ptr<Texture> output);
    
public:
    
    List< Ptr<LetterCandidate> > LetterCandidates;
    
private:
    
    Ptr<Filter> grayFilter;
    //Ptr<SWTFilter> swtFilter;
    //Ptr<ConnectedComponentsFilter> connectedComponentsFilter;
    Ptr<Texture> gray, stencil;
    Ptr<Program> boundingBoxes, filterInvalidComponents, filterComponentProperties, countComponents, stencilRouting, countPixels, average, variance, writeIDs;
    // todo: for debug purposes
    Ptr<Program> vertexTexture;
};

inline TextRegionsFilter::TextRegionsFilter(Ptr<Texture> input)
    : base("TextRegions", input)
{
    
}