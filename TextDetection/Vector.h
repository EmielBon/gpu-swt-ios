//
//  Vector.h
//  TextDetection
//
//  Created by Emiel Bon on 16-07-14.
//  Copyright (c) 2014 Emiel Bon. All rights reserved.
//

#pragma once

#include <math.h>

#import <opencv2/opencv.hpp>

using Vector2  = cv::Vec2f;
using Vector2i = cv::Vec2i;
using Vector3  = cv::Vec3f;
using Vector3i = cv::Vec3i;
using Vector4  = cv::Vec4f;
using Vector4i = cv::Vec4i;

/*template<class T, int N>
struct Vector
{
    Vector() = default;
    
    Vector(const std::initializer_list<T> &elements)
    {
        auto it = elements.begin();
        for(int i = 0; i < N; ++i)
            elems[i] = *it++;
    }
    
    T& operator[](int i)
    {
        return elems[i];
    }
    
    const T& operator[](int i) const
    {
        return elems[i];
    }
    
    T lengthSquared() const
    {
        T lengthSum = 0;
        for (int i = 0; i < N; ++i)
            lengthSum += elems[i] * elems[i];
        return lengthSum;
    }
    
    float length() const
    {
        return sqrtf( lengthSquared() );
    }
    
    static Vector<T, N> normalize(const Vector<T, N> &v)
    {
        float l = v.length();
        Vector<T, N> copy;
        for (int i = 0; i < N; ++i)
            copy[i] = v[i] / l;
        return copy;
    }
    
    Vector<T, N> normalize() const
    {
        return normalize(*this);
    }
    
    T dot(const Vector<T, N> &other) const
    {
        T sum = 0;
        for (int i = 0; i < N; ++i)
            sum += elems[i] * other[i];
        return sum;
    }
    
    T elems[N];
};

using Vector2  = Vector<float, 2>;
using Vector2i = Vector<int,   2>;
using Vector3  = Vector<float, 3>;
using Vector3i = Vector<int,   3>;
using Vector4  = Vector<float, 4>;
using Vector4i = Vector<int,   4>;*/
using Point    = Vector2i;