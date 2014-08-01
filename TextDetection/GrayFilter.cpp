//
//  GrayFilter.cpp
//  OGLTest
//
//  Created by Emiel Bon on 22-04-14.
//  Copyright (c) 2014 Emiel Bon. All rights reserved.
//

#include "GrayFilter.h"
#include "VertexPosition.h"
#include "VertexBuffer.h"
#include "Texture.h"
#include "GraphicsDevice.h"
#include "FrameBuffer.h"

Ptr<Texture> GrayFilter::PerformSteps()
{
    auto output = New<Texture>(GL_RED_EXT, GL_UNSIGNED_BYTE);
    
    grayscale->Use();
    grayscale->Uniforms["Texture"].SetValue(*Input);
    RenderToTexture(output, PrimitiveType::Triangles);
    
    return output;
}