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

void GraphicsDevice::DrawElements(PrimitiveType primitiveType)
{
    AssertCompleteProgram();
    VertexBuffer->VertexArrayObject.Bind();
    IndexBuffer->Bind(); // apparently HAS to happen AFTER vertex array setup
    glDrawElements((GLenum)primitiveType, IndexBuffer->Count(), GL_UNSIGNED_BYTE, NULL);
    IndexBuffer->Unbind();
}

void GraphicsDevice::DrawArrays(PrimitiveType primitiveType)
{
    AssertCompleteProgram();
    VertexBuffer->VertexArrayObject.Bind();
    // Draw without index buffer
    glDrawArrays((GLenum)primitiveType, 0, VertexBuffer->Count());
    VertexArray::BindDefault();
}

/*void GraphicsDevice::SetupVertexArray()
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
}*/

void GraphicsDevice::AssertCompleteProgram()
{
    assert(Program::GetCurrentlyUsed());
    
    for (auto& keyValues : Program::GetCurrentlyUsed()->Uniforms)
    {
        auto &uniform = keyValues.second;
        assert2(uniform.IsInitialized(), "Error: No value specified for uniform \"" + uniform.GetName() + "\"");
    }
}