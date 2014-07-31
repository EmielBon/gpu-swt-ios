//
//  SWTHelper.cpp
//  OGLTest
//
//  Created by Emiel Bon on 25-03-14.
//  Copyright (c) 2014 Emiel Bon. All rights reserved.
//

#include "SWTHelper.h"
#include "GraphicsDevice.h"
#include "FrameBuffer.h"
#include "ContentLoader.h"
#include "Texture.h"
#include "DrawableRect.h"
#include "RenderWindow.h"
#include "BoundingBox.h"
#include "Profiling.h"
#include "TextRegionsFilter.h"
#include "VertexPosition.h"
#include "RenderBuffer.h"
#include "RenderBufferType.h"
#include "LetterCandidate.h"
#include "Chain.h"
#include "SWTParameters.h"

List<Ptr<LetterCandidate>> SWTHelper::FilterOnOverlappingBoundingBoxes(List<Ptr<LetterCandidate>> &letterCandidates)
{
    List<Ptr<LetterCandidate>> filtered(letterCandidates.size());
    
    auto end = std::copy_if(letterCandidates.begin(), letterCandidates.end(), filtered.begin(), [&] (Ptr<LetterCandidate> c1) {
        int overlapCount = 0;
        for(auto c2 : letterCandidates)
        {
            if (c1 == c2) continue;
            if (c1->BoundingBox.Contains(c2->BoundingBox) != ContainmentType::Disjoint)
                overlapCount++;
        }
        return (overlapCount <= 2);
    });
    
    filtered.resize(std::distance(filtered.begin(), end));
    return filtered;
}

LinkedList< Ptr<Chain> > SWTHelper::MakePairs(List<Ptr<LetterCandidate>> &components)
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

void SWTHelper::FilterNonWords(LinkedList< Ptr<Chain> > &chains)
{
    auto end = std::remove_if(chains.begin(), chains.end(), [] (Ptr<Chain> chain) {
        auto chainLength = chain->LetterCandidates.size();
        return (chainLength < MinChainLength || chainLength > MaxChainLength);
    });
    chains.resize(std::distance(chains.begin(), end));
}

Ptr<LetterCandidate> SWTHelper::GetSharedComponent(const Chain& chain1, const Chain& chain2)
{
    for(auto c1 : chain1.LetterCandidates)
        for(auto c2 : chain2.LetterCandidates)
            if (c1 == c2) return c1;
    return nullptr;
}

bool SWTHelper::ChainingIteration(LinkedList<Ptr<Chain>> &chains)
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

float SWTHelper::CalculateSimilarity(const Chain &c1, const Chain &c2)
{
    auto shared = GetSharedComponent(c1, c2);
    if (!shared)
        return 0;
    
    float angle = acos(c1.Direction().dot(c2.Direction()));
    
	if (angle >= (M_PI / 8))
		return 0;
	return 1 - (angle / (M_PI / 2));
}