//
//  PrimitiveType.h
//  OGLTest
//
//  Created by Emiel Bon on 06-02-14.
//  Copyright (c) 2014 Emiel Bon. All rights reserved.
//

#pragma once

#include <OpenGLES/ES2/gl.h>

#ifndef GL_QUADS
    #define GL_QUADS 0x0007
#endif

enum class PrimitiveType : GLenum
{
    Points      = GL_POINTS,
    Lines       = GL_LINES,
    Triangles   = GL_TRIANGLES,
    Unspecified = GL_QUADS,
};