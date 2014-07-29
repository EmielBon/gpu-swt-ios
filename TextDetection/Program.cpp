//
//  Program.cpp
//  OGLTest
//
//  Created by Emiel Bon on 03-02-14.
//  Copyright (c) 2014 Emiel Bon. All rights reserved.
//

#include "Program.h"

#include "ContentLoader.h"
#include "GraphicsDevice.h"
#include "Shader.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexDeclaration.h"

Program* Program::currentlyUsedProgram = nullptr;

Program::Program(const List< Ptr<Shader>> &shaders) : programId(0)
{
    programId = CreateFromShaders(shaders);
    AssertLinkingSuccess(programId);
    IndexUniforms();
}

GLuint Program::CreateFromShaders(const List<Ptr<Shader> > &shaders)
{
    assert(shaders.size() >= 2);
    
    //create the program object
    GLuint programId = glCreateProgram();
    
    assert(programId);
    
    //attach all the shaders
    for(unsigned i = 0; i < shaders.size(); ++i)
        glAttachShader(programId, shaders[i]->GetHandle());
    
    // Hardcode position to attribute location 0, I don't like it but I hope it helps with the VAO problems
    glBindAttribLocation(programId, 0, "Position");
    
    //link the shaders together
    glLinkProgram(programId);
    
    //detach all the shaders
    for(unsigned i = 0; i < shaders.size(); ++i)
        glDetachShader(programId, shaders[i]->GetHandle());
    
    return programId;
}

void Program::AssertLinkingSuccess(GLuint programId)
{
    //throw exception if linking failed
    GLint status;
    glGetProgramiv(programId, GL_LINK_STATUS, &status);
    if (status == GL_FALSE) {
        String msg("Program linking failure: ");
        
        GLint infoLogLength;
        glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &infoLogLength);
        char* strInfoLog = new char[infoLogLength + 1];
        glGetProgramInfoLog(programId, infoLogLength, NULL, strInfoLog);
        msg += strInfoLog;
        delete[] strInfoLog;
        
        glDeleteProgram(programId);
        
        crash(msg);
    }
}

void Program::IndexUniforms()
{
    int total = -1;
    int textureIndex = 0;
    
    glGetProgramiv( programId, GL_ACTIVE_UNIFORMS, &total );
    for(int i = 0; i < total; ++i)  {
        int nameLength = -1, size = -1;
        GLenum type = GL_ZERO;
        char name[100];
        glGetActiveUniform( programId, (GLuint)i, sizeof(name)-1,
                           &nameLength, &size, &type, name );
        name[nameLength] = 0;
        GLuint location = glGetUniformLocation( programId, name );
        
        Uniforms[name] = Uniform( name, location, type, (type == GL_SAMPLER_2D) ? textureIndex++ : 0);
    }
}

GLint Program::GetAttributeLocation(const String &name) const
{
    GLint attrib = glGetAttribLocation(programId, name.c_str());
    assert2(attrib >= -1, "Program attribute not found: " + name);
    return attrib;
}

void Program::Use()
{
    currentlyUsedProgram = this;
    glUseProgram(programId);
}

Ptr<Program> Program::LoadScreenSpaceProgram(const String &fragmentShaderSource)
{
    return ContentLoader::Load<Program>("Trivial", fragmentShaderSource);
}