//
//  Uniform.cpp
//  OGLTest
//
//  Created by Emiel Bon on 07-02-14.
//  Copyright (c) 2014 Emiel Bon. All rights reserved.
//

#include "Uniform.h"
#include "Texture.h"

void Uniform::SetValue(const Texture &texture)
{
    AssertLocation();
    initialized = true;
    glActiveTexture(GL_TEXTURE0 + index); check_gl_error();
    texture.Bind(); check_gl_error();
    SetValue(index); check_gl_error();
}