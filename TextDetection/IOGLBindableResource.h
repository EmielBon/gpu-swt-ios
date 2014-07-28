//
//  IOGLBindableResource.h
//  TextDetection
//
//  Created by Emiel Bon on 28-07-14.
//  Copyright (c) 2014 Emiel Bon. All rights reserved.
//



#pragma once

#include "IOGLResource.h"
#include "types.h"

class IOGLBindableResource
{
public:
    
    virtual void Bind() const = 0;
    
    virtual void Unbind() const = 0;
    
    virtual bool IsBound() const = 0;
};