//
//  DepthBuffer.h
//  OGLTest
//
//  Created by Emiel Bon on 18-03-14.
//  Copyright (c) 2014 Emiel Bon. All rights reserved.
//

#pragma once

#include "OGLBindableResource.h"
#include "RenderBufferType.h"
#include "types.h"

class RenderBuffer : public OGLBindableResource<RenderBuffer>
{
public:
    
    RenderBuffer(int width, int height, RenderBufferType type);
    
public:
    
    RenderBufferType BufferType;
};