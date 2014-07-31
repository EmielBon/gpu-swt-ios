//
//  SWTHelperGPU.h
//  OGLTest
//
//  Created by Emiel Bon on 25-03-14.
//  Copyright (c) 2014 Emiel Bon. All rights reserved.
//

#pragma once

#include "types.h"

class SWTHelper
{
public:
    
    static List<Ptr<LetterCandidate>> FilterOnOverlappingBoundingBoxes(List<Ptr<LetterCandidate>> &components);
    
    static LinkedList< Ptr<Chain> > MakePairs(List<Ptr<LetterCandidate>> &components);
    
    static void FilterNonWords(LinkedList< Ptr<Chain> > &chains);
    
    static Ptr<LetterCandidate> GetSharedComponent(const Chain& chain1, const Chain& chain2);
    
    static bool ChainingIteration(LinkedList<Ptr<Chain>> &chains);
    
    static float CalculateSimilarity(const Chain &c1, const Chain &c2);
};