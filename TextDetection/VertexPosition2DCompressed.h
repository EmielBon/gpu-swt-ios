//
//  VertexPositionCompressed.h
//  OGLTest
//
//  Created by Emiel Bon on 19-06-14.
//  Copyright (c) 2014 Emiel Bon. All rights reserved.
//

#pragma once

#include "types.h"

struct VertexPosition2DCompressed
{
public:
    
    VertexPosition2DCompressed() = default;
    
    VertexPosition2DCompressed(const Vector2i &position);
    
    static const VertexDeclaration& VertexDeclaration();

public:
    
    Vector2i GetPosition() const;
    
    void SetPosition(const Vector2i &position);
    
    static float Encode(const Vector2i &position);
    
    static Vector2i Decode(float encodedPosition);
    
public:
    
    float EncodedPosition;
};

inline VertexPosition2DCompressed::VertexPosition2DCompressed(const Vector2i &position)
{
    SetPosition(position);
}

inline Vector2i VertexPosition2DCompressed::GetPosition() const
{
    return Decode(EncodedPosition);
}

inline void VertexPosition2DCompressed::SetPosition(const Vector2i &position)
{
    EncodedPosition = Encode(position);
}

inline float VertexPosition2DCompressed::Encode(const Vector2i &position)
{
    int ret = (position[0] + 1) | ((position[1] + 1) << 11);
    return float(ret);
}

inline Vector2i VertexPosition2DCompressed::Decode(float encodedPosition)
{
    int i = int(encodedPosition);
    int x = (i & 0x000007FF) - 1;
    int y = ((i & 0x003FF800) >> 11) - 1;
    return {x, y};
}