//
//  Chain.cpp
//  OGLTest
//
//  Created by Emiel Bon on 28-02-14.
//  Copyright (c) 2014 Emiel Bon. All rights reserved.
//

#include "Chain.h"

#import <opencv2/opencv.hpp>

Chain::Chain(const std::initializer_list< Ptr<LetterCandidate> > &components) : Chain()
{
    assert(components.size() >= 2);
    LetterCandidates.insert(components.begin(), components.end());
}

Vector2 Chain::Direction(bool normalize) const
{
    assert(LetterCandidates.size() >= 2);
    
    auto left = LetterCandidates.begin();
    auto right = std::prev(LetterCandidates.end());
    
    assert(left != right);
    
    Point p1 =  (*left)->BoundingBox.Center();
    Point p2 = (*std::next(left))->BoundingBox.Center();
    
    assert(p1[0] < p2[0] || (p1[0] == p2[0] && p1[1] <= p2[1]));
    
    Vector2 direction = { (float)(p2[0] - p1[0]), (float)(p2[1] - p1[1]) };
    
    if (LetterCandidates.size() > 2)
    {
        for(auto it = std::next(left); it != std::prev(right); ++it)
        {
            auto next = std::next(it);
            Point p1 =   (*it)->BoundingBox.Center();
            Point p2 = (*next)->BoundingBox.Center();
            
            assert(p1[0] < p2[0] || (p1[0] == p2[0] && p1[1] < p2[1]));
            
            if (it != left)
            {
                Vector2 dir1 = cv::normalize(direction);
                Vector2 dir2 = cv::normalize(Vector2( (float)(p2[0] - p1[0]), (float)(p2[1] - p1[1])));
                float dot = dir1.dot(dir2);
            }
            direction += Vector2( (float)(p2[0] - p1[0]), (float)(p2[1] - p1[1]) );
        }
    }
    return normalize ? cv::normalize(direction) : direction;
}

float Chain::PolarAngle() const
{
    auto direction = Direction(false);
    assert(atan2f(direction[1], direction[0]) < M_PI);
    return atan2f(direction[1], direction[0]);
}

Ptr<Chain> Chain::Merge(const Chain &chain1, const Chain &chain2)
{
    Ptr<Chain> merged(new Chain());
    merged->LetterCandidates.insert(chain1.LetterCandidates.begin(), chain1.LetterCandidates.end());
    merged->LetterCandidates.insert(chain2.LetterCandidates.begin(), chain2.LetterCandidates.end());
    return merged;
}

BoundingBox Chain::BoundingBox() const
{
    int x1 = INT_MAX, y1 = INT_MAX, x2 = 0, y2 = 0;
    
    for(auto c : LetterCandidates)
    {
        auto& bbox = c->BoundingBox;
        x1 = std::min(x1, bbox.X());
        y1 = std::min(y1, bbox.Y());
        x2 = std::max(x2, bbox.X() + bbox.Width());
        y2 = std::max(y2, bbox.Y() + bbox.Height());
    }
    
    return ::BoundingBox( cv::Rect(x1, y1, x2 - x1, y2 - y1) );
}
