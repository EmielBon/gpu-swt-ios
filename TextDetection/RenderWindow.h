//
//  Renderer.h
//  OGLTest
//
//  Created by Emiel Bon on 31-01-14.
//  Copyright (c) 2014 Emiel Bon. All rights reserved.
//

#pragma once

#include <memory>
#include "ContainerTypes.h"
#include "CoreTypes.h"
#include "FrameworkTypes.h"
#include "GraphicsTypes.h"

class RenderWindow
{
public:
    
    RenderWindow(GLuint inputTextureHandle, GLuint width, GLuint height);
    
    static RenderWindow& Instance();
    
    void DrawRect(const DrawableRect &rect);
    
    void DrawTexture(int index);
    
    const char* GetTextureName(int index);
    
    void AddTexture(std::shared_ptr<Texture> input, const String &descriptor = "", bool makeCopy = true);
    
    void AddFrameBufferSnapshot(const String &descriptor = "");
    
private:
    
    std::shared_ptr<Program>       program;
    std::shared_ptr<DrawableRect>  rect1;
    List<std::shared_ptr<Texture>> textures;
    List<String>                   textureDescriptors;
    int                            currentTextureIndex, oldTextureIndex;
    
    static RenderWindow* instance;
};

inline RenderWindow& RenderWindow::Instance()
{
    return *instance;
}