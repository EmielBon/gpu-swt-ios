//
//  Component.h
//  OGLTest
//
//  Created by Emiel Bon on 24-02-14.
//  Copyright (c) 2014 Emiel Bon. All rights reserved.
//

#pragma once

#include "types.h"
#include "BoundingBox.h"

struct LetterCandidate
{
public:
    
    LetterCandidate() = default;
    
    // Can be used with std::sort to sort components from left-to-right, top-to-bottom
    static bool HasSmallerPosition(Ptr<LetterCandidate> c1, Ptr<LetterCandidate> c2);
    
    bool CanLinkWith(const LetterCandidate &other);
    
public:
    
    cv::Vec3b MeanColor;
    float MeanStrokeWidth;
    ::BoundingBox BoundingBox;
};