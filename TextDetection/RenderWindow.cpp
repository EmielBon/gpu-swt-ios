//
//  Renderer.cpp
//  OGLTest
//
//  Created by Emiel Bon on 31-01-14.
//  Copyright (c) 2014 Emiel Bon. All rights reserved.
//

#include "RenderWindow.h"

// Framework
#include "BoundingBox.h"
#include "ContentLoader.h"
#include "DrawableRect.h"
// Graphics
#include "FrameBuffer.h"
#include "GLError.h"
#include "GraphicsDevice.h"
#include "Program.h"
#include "Texture.h"
// SWT stuff
#include "Chain.h"
#include "LetterCandidate.h"
#include "SWTParameters.h"
#include "SWTHelperGPU.h"
// Misc
#include "types.h"
#include <assert.h>
#include <stdio.h>

void FilterOnOverlappingBoundingBoxes(List<Ptr<LetterCandidate>> &components);
void FilterNonWords(LinkedList< Ptr<Chain> > &chains);
float CalculateSimilarity(const Chain& c1, const Chain &c2);
bool ChainingIteration(LinkedList<Ptr<Chain>> &chains);
/*void DrawChains(const cv::Mat &input, const LinkedList< Ptr<Chain> > &components, const String &description);*/
LinkedList< Ptr<Chain> > MakePairs(List<Ptr<LetterCandidate>> &components);

RenderWindow* RenderWindow::instance = nullptr;

RenderWindow::RenderWindow(GLuint inputTextureHandle, GLuint width, GLuint height)
    : currentTextureIndex(0), oldTextureIndex(-1)
{
    instance = this;
        
    //auto input = ContentLoader::Load<Texture>("sign800x600");
    
    auto input = New<Texture>(inputTextureHandle, width, height, GL_RGBA, GL_UNSIGNED_BYTE); // These parameters probably have no impact, except if you want an empty copy of this texture
    check_gl_error();
    AddTexture(input, "Input image", false);
    check_gl_error();
    rect1 = New<DrawableRect>(-1, -1, 1, 1);
    GraphicsDevice::SetDefaultBuffers(rect1->VertexBuffer, rect1->IndexBuffer);
    GraphicsDevice::UseDefaultBuffers();
    check_gl_error();
    program = Program::LoadScreenSpaceProgram("Sobel1");
    check_gl_error();
    auto letterCandidates = SWTHelperGPU::StrokeWidthTransform(input);
    check_gl_error();
    /*FilterOnOverlappingBoundingBoxes(letterCandidates);
    //DrawBoundingBoxes(input, components, "Boundingboxes with overlap <= 2 (letters)");
    
    auto chains = MakePairs(letterCandidates);
    DrawChains(input, chains, "Paired components");
    
    while(ChainingIteration(chains))
        ;
    DrawChains(input, chains, "Chains");
    
    FilterNonWords(chains);
    DrawChains(input, chains, "Chains with length >= 3 (words)");
    
    List<BoundingBox> boundingBoxes;
    
    for(auto chain : chains)
        boundingBoxes.push_back(chain->BoundingBox());
    
    cv::Mat output = ImgProc::DrawBoundingBoxes(input, boundingBoxes, {0, 255, 255, 255});
    AddTexture(output, "Detected text regions");*/
}

void RenderWindow::Draw()
{
    glViewport(0, 0, SWTHelperGPU::InputWidth, SWTHelperGPU::InputHeight);
    check_gl_error();
    
    if (!textures.empty())
    {
        /*
         static bool keyPressed = false;
         
         if (glfwGetKey(window, GLFW_KEY_RIGHT) && !keyPressed)
         {
         currentTextureIndex = (currentTextureIndex + 1) % textures.size();
         keyPressed = true;
         glfwSetWindowTitle(window, textureDescriptors[currentTextureIndex].c_str());
         }
         if (glfwGetKey(window, GLFW_KEY_LEFT) && !keyPressed)
         {
         currentTextureIndex--;
         if (currentTextureIndex < 0)
         currentTextureIndex += textures.size();
         keyPressed = true;
         glfwSetWindowTitle(window, textureDescriptors[currentTextureIndex].c_str());
         }
         if (!glfwGetKey(window, GLFW_KEY_RIGHT) && !glfwGetKey(window, GLFW_KEY_LEFT))
         {
         keyPressed = false;
         }
         */
        DrawCurrentTexture();
    }
    check_gl_error();
}

void RenderWindow::DrawCurrentTexture()
{
    static float counter = 0;
    counter += 0.1f;
    if (counter > 1)
    {
        counter = 0;
        currentTextureIndex = (currentTextureIndex + 1) % textures.size();
    }
    glClearColor(1, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    check_gl_error();
    GraphicsDevice::UseDefaultBuffers();
    check_gl_error();
    auto texture = textures[currentTextureIndex];
    check_gl_error();
    program->Use();
    check_gl_error();
    program->Uniforms["Texture"].SetValue(*texture);
    check_gl_error();
    if (!GraphicsDevice::IndexBuffer)
        GraphicsDevice::DrawArrays(PrimitiveType::Triangles);
    else
        GraphicsDevice::DrawPrimitives(PrimitiveType::Triangles);
    check_gl_error();
}

void RenderWindow::AddTexture(Ptr<Texture> input, const String &descriptor /* = "" */, bool makeCopy /* = true */)
{
    if (!input)
        return;
    Ptr<Texture> texture = input;
    
    /*if (makeCopy)
    {
        GraphicsDevice::UseDefaultBuffers();
        auto x = GraphicsDevice::VertexBuffer;
        auto y = GraphicsDevice::IndexBuffer;
        texture = input->GetEmptyClone();
        auto previousColorAttachment = FrameBuffer::DefaultOffscreenFrameBuffer->ColorAttachment0;
        FrameBuffer::DefaultOffscreenFrameBuffer->SetColorAttachment(texture);
        program->Use();
        program->Uniforms["Texture"].SetValue(*input);
        GraphicsDevice::DrawArrays(PrimitiveType::Triangles);
        FrameBuffer::DefaultOffscreenFrameBuffer->SetColorAttachment(previousColorAttachment);
    }*/
    textures.push_back(texture);
    textureDescriptors.push_back(descriptor);
}

void FilterOnOverlappingBoundingBoxes(List<Ptr<LetterCandidate>> &components)
{
    List<Ptr<LetterCandidate>> filtered(components.size());
    
    auto end = std::copy_if(components.begin(), components.end(), filtered.begin(), [&] (Ptr<LetterCandidate> c1) {
        int overlapCount = 0;
        for(auto c2 : components)
        {
            if (c1 == c2) continue;
            if (c1->BoundingBox.Contains(c2->BoundingBox) != ContainmentType::Disjoint)
                overlapCount++;
        }
        return (overlapCount <= 2);
    });
    
    filtered.resize(std::distance(filtered.begin(), end));
    components = filtered;
}

LinkedList< Ptr<Chain> > MakePairs(List<Ptr<LetterCandidate>> &components)
{
    LinkedList< Ptr<Chain> > chains;
    
    for(auto c1 : components)
    {
        for(auto c2 : components)
        {
            if (c1 == c2)
                continue;
            if (!c1->CanLinkWith(*c2))
                continue;
            
            auto pair = Ptr<Chain>( new Chain({c1, c2}) );
            
            // Throw away non (near-)horizontal pairs
            if (acos(pair->Direction().dot( {1.0f, 0.0f} )) > MaxNullAngleDifference)
                continue;
            
            chains.push_back(pair);
        }
    }
    return chains;
}

void FilterNonWords(LinkedList< Ptr<Chain> > &chains)
{
    auto end = std::remove_if(chains.begin(), chains.end(), [] (Ptr<Chain> chain) {
        auto chainLength = chain->LetterCandidates.size();
        return (chainLength < MinChainLength || chainLength > MaxChainLength);
    });
    chains.resize(std::distance(chains.begin(), end));
}

Ptr<LetterCandidate> GetSharedComponent(const Chain& chain1, const Chain& chain2)
{
    for(auto c1 : chain1.LetterCandidates)
        for(auto c2 : chain2.LetterCandidates)
            if (c1 == c2) return c1;
    return nullptr;
}

bool ChainingIteration(LinkedList<Ptr<Chain>> &chains)
{
    using Couple = Tuple<Ptr<Chain>, Ptr<Chain>>;
    Couple lastCouple = {nullptr, nullptr};
    
    float highestSimilarity = 0;
    
    for(auto it = chains.begin(); it != chains.end(); ++it)
    {
        for(auto it2 = std::next(it); it2 != chains.end(); ++it2)
        {
            auto c1 = *it;
            auto c2 = *it2;
            
            float similarity = CalculateSimilarity(*c1, *c2);
            
            if (similarity > highestSimilarity)
            {
                highestSimilarity = similarity;
                lastCouple = {c1, c2};
            }
        }
    }
    
    if (highestSimilarity > 0)
    {
        chains.push_back( Chain::Merge(*std::get<0>(lastCouple), *std::get<1>(lastCouple)) );
        chains.remove(std::get<0>(lastCouple));
        chains.remove(std::get<1>(lastCouple));
    }
    
    return (highestSimilarity > 0);
}

float CalculateSimilarity(const Chain &c1, const Chain &c2)
{
    auto shared = GetSharedComponent(c1, c2);
    if (!shared)
        return 0;
    
    float angle = acos(c1.Direction().dot(c2.Direction()));
    
	if (angle >= (M_PI / 8))
		return 0;
	return 1 - (angle / (M_PI / 2));
}

/*void DrawChains(const cv::Mat &input, const LinkedList< Ptr<Chain> > &chains, const String &description)
{
    cv::Mat lineImage = input.clone();
    for(auto chain : chains)
    {
        for(auto it = chain->LetterCandidates.begin(); it != chain->LetterCandidates.end(); ++it)
        {
            auto c1 = *it;
            
            cv::rectangle(lineImage, c1->BoundingBox.Bounds, {0, 255, 255, 255});
            
            if (std::next(it) == chain->LetterCandidates.end())
                break;
            
            auto c2 = *std::next(it);
            
            cv::line(lineImage, c1->BoundingBox.Center(), c2->BoundingBox.Center(), {0, 255, 0, 255}, 1);
        }
    }
    RenderWindow::Instance().AddTexture(lineImage, description);
}

void RenderWindow::SetWindowSize(const cv::Size &size, const cv::Size &max)
{
    float aspectRatio = size.width / (float)size.height;
    int width = std::min(size.width, max.width);
    //int height = std::min(size.height, max.height);
    
    glfwSetWindowSize(window, width, width / aspectRatio);
}
*/