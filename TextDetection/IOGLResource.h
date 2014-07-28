//
//  IOGLResource.h
//  TextDetection
//
//  Created by Emiel Bon on 28-07-14.
//  Copyright (c) 2014 Emiel Bon. All rights reserved.
//

#pragma once

#include "types.h"

class IOGLResource
{
public:
    
    virtual void Generate() = 0;
    
    virtual GLuint GetHandle() const = 0;
    
    virtual void Dispose() = 0;
};

