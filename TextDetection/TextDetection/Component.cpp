//
//  Component.cpp
//  OGLTest
//
//  Created by Emiel Bon on 24-02-14.
//  Copyright (c) 2014 Emiel Bon. All rights reserved.
//

#include "Component.h"
#include "MathHelper.h"
//#include "ImgProc.h"
#include "SWTParameters.h"
#include <numeric>

void Component::Update()
{
    if (!dirty) return;
    boundingBox = cv::boundingRect(points);
    dirty = false;
}

void Component::SetMeanStrokeWidthFromImage(const cv::Mat &image)
{
    float sum = 0;
    for(auto &p : points)
        sum += image.at<float>(p[1], p[0]);
    meanStrokeWidth = sum / points.size();
    sum = 0;
    for(auto &p : points)
    {
        float d = image.at<float>(p[1], p[0]) - meanStrokeWidth;
        sum += d * d;
    }
    strokeWidthVariance = sum / points.size();
}

void Component::SetMeanColorFromImage(const cv::Mat &image)
{
    cv::Vec3i sum(0, 0, 0);
    for(auto &p : points)
    {
        auto &c = image.at<cv::Vec3b>(p[1], p[0]);
        sum[0] += c[0];
        sum[1] += c[1];
        sum[2] += c[2];
    }
    sum[0] /= points.size();
    sum[1] /= points.size();
    sum[2] /= points.size();
    
    assert(sum[0] <= 255 && sum[1] <= 255 && sum[2] <= 255);
    
    meanColor = cv::Vec3b(sum[0], sum[1], sum[2]);
}

bool Component::IsValid() const
{
    float aspectRatio = (float)BoundingBox().Width() / BoundingBox().Height();
    float mean = GetMeanStrokeWidth();
    float occupancy = GetOccupancy();
    float variance = GetStrokeWidthVariance();
    
    // todo: have to make width and height some sort of scale
    return (aspectRatio >= MinAspectRatio && aspectRatio <= MaxAspectRatio &&
            occupancy   >= MinOccupancy   &&
           (aspectRatio < 1 || occupancy <= MaxOccupancy)   &&
            variance <= mean / MaxVarianceToMeanRatio);
}