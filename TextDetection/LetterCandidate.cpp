//
//  LetterCandidate.cpp
//  OGLTest
//
//  Created by Emiel Bon on 14-07-14.
//  Copyright (c) 2014 Emiel Bon. All rights reserved.
//

#include "LetterCandidate.h"
#include "MathHelper.h"
#include "SWTParameters.h"
#include <numeric>

bool LetterCandidate::HasSmallerPosition(Ptr<LetterCandidate> c1, Ptr<LetterCandidate> c2)
{
    Point p1 = c1->BoundingBox.BaseLineCenter();
    Point p2 = c2->BoundingBox.BaseLineCenter();
    
    return (p1[0] == p2[0]) ? (p1[1] < p2[1]) : (p1[0] < p2[0]);
}

bool LetterCandidate::CanLinkWith(const LetterCandidate &other)
{
    if (MathHelper::GetRatio(MeanStrokeWidth, other.MeanStrokeWidth) >= MaxComponentSWTRatio)
        return false;
    if (MathHelper::GetRatio(BoundingBox.Height(), other.BoundingBox.Height()) >= MaxComponentHeightRatio)
        return false;
    if (MathHelper::Length((BoundingBox.Center() - other.BoundingBox.Center())) >= MaxComponentDistanceRatio * std::max(BoundingBox.Width(), other.BoundingBox.Width()))
        return false;
    
    // todo: maybe use Delta-E?
    cv::Vec3b color1 = MeanColor;
    cv::Vec3b color2 = other.MeanColor;
    
    float c1 = color1[0] * 0.30f + color1[1] * 0.59 + color1[2] * 0.11;
    float c2 = color2[0] * 0.30f + color2[1] * 0.59 + color2[2] * 0.11;
    
    if (fabs(c1 - c2) / 255.0f >= MaxColorDifference)
        return false;
    
    return true;
}