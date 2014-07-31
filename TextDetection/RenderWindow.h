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
#include "SWTTypes.h"

class RenderWindow
{
public:
    
    RenderWindow(GLuint inputTextureHandle, GLuint width, GLuint height);
    
    void SetupGL();
    
    static RenderWindow& Instance();
    
    void DisableIrrelvantState();
    
    void DrawRect(const DrawableRect &rect);
    
    void DrawTexture(int index);
    
    const char* GetTextureName(int index);
    
    void AddTexture(std::shared_ptr<Texture> input, const String &descriptor = "");
    
    void AddFrameBufferSnapshot(const String &descriptor = "");
    
    List<std::shared_ptr<LetterCandidate>> PerformStrokeWidthTransformGPU(std::shared_ptr<Texture> input);
    
private:
    
    std::shared_ptr<Program>       program;
    std::shared_ptr<DrawableRect>  rect1;
    List<std::shared_ptr<Texture>> textures;
    List<String>                   textureDescriptors;
    int                            currentTextureIndex, oldTextureIndex;
    
public:
    
    static RenderWindow* instance;
    static TimeSpan RenderTime, CompileTime;
};

inline RenderWindow& RenderWindow::Instance()
{
    return *instance;
}