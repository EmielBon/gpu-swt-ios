//
//  Rect.h
//  TextDetection
//
//  Created by Emiel Bon on 16-07-14.
//  Copyright (c) 2014 Emiel Bon. All rights reserved.
//

#pragma once

#include "Vector.h"

using Rect = cv::Rect;

/*struct Rect
{
    Rect(int x, int y, int width, int height) : x(x), y(y), width(width), height(height)
    {
        
    }
    
    int area() const
    {
        return width * height;
    }
    
    bool contains(const Vector2i &point) const
    {
        return x <= point[0] && point[0] < x + width && y <= point[1] && point[1] < y + height;
    }
    
    Vector2i tl() const
    {
        return {x, y};
    }
    
    Vector2i br() const
    {
        return {x + width, y + height};
    }
    
    int x, y, width, height;
};*/