//
//  GraphicsDevice.cpp
//  OGLTest
//
//  Created by Emiel Bon on 06-02-14.
//  Copyright (c) 2014 Emiel Bon. All rights reserved.
//

#include "GraphicsDevice.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "Program.h"

Ptr<::VertexBuffer> GraphicsDevice::VertexBuffer = nullptr;
Ptr<::IndexBuffer>  GraphicsDevice::IndexBuffer  = nullptr;

Ptr<::VertexBuffer> GraphicsDevice::DefaultVertexBuffer = nullptr;
Ptr<::IndexBuffer>  GraphicsDevice::DefaultIndexBuffer  = nullptr;

void GraphicsDevice::DrawPrimitives()
{
    AssertCompleteProgram();
    SetupVertexArray();
    // bind the index buffer (apparently HAS to happen AFTER vertex array setup)
    IndexBuffer->Bind();
    // draw the VAO
    // todo: kan miss een kleiner datatype dan int of zelfs short, omdat er voor per pixel geen indices gebruikt worden
    glDrawElements((GLenum)(IndexBuffer->PrimitiveType), IndexBuffer->Count(), GL_UNSIGNED_BYTE, NULL);
}

void GraphicsDevice::DrawArrays(PrimitiveType primitiveType)
{
    check_gl_error();
    AssertCompleteProgram();
    check_gl_error();
    SetupVertexArray();
    check_gl_error();
    // Draw without index buffer
    glDrawArrays((GLenum)primitiveType, 0, VertexBuffer->Count());
}

void GraphicsDevice::SetupVertexArray()
{
    auto &vertexDeclaration         = VertexBuffer->GetVertexDeclaration();
    auto &vertexDeclarationElements = vertexDeclaration.GetElements();
    GLuint stride                   = vertexDeclaration.Stride();
    
    VertexBuffer->Bind();
    VertexBuffer->VertexArrayObject.Bind();
    
    auto activeProgram = Program::GetCurrentlyUsed();
    
    // Can be optimized/avoided by caching
    for(auto vertexElement : vertexDeclarationElements)
    {
        GLint  location = activeProgram->GetAttributeLocation(vertexElement.GetName().c_str());
        GLuint size     = vertexElement.GetSize();
        GLenum type     = vertexElement.GetType();
        
        glEnableVertexAttribArray(location);
        glVertexAttribPointer(location, size, type, GL_FALSE, stride, reinterpret_cast<void*>(vertexElement.GetElementOffset()));
    }
}

void GraphicsDevice::AssertCompleteProgram()
{
    if (Program::GetCurrentlyUsed() == nullptr)
        throw std::runtime_error("Error: Rendering without program");
        
    for (auto& keyValues : Program::GetCurrentlyUsed()->Uniforms)
    {
        auto &uniform = keyValues.second;
        if (!uniform.IsInitialized())
            throw std::runtime_error("Error: No value specified for uniform \"" + uniform.GetName() + "\"");
    }
}