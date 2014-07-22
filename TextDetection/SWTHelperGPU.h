//
//  SWTHelperGPU.h
//  OGLTest
//
//  Created by Emiel Bon on 25-03-14.
//  Copyright (c) 2014 Emiel Bon. All rights reserved.
//

#pragma once

#include "types.h"

class SWTHelperGPU
{
public:
    
    static List<Ptr<LetterCandidate>> StrokeWidthTransform(Ptr<Texture> input);
    
private:
    
    static Ptr<Texture> ApplyPass(Ptr<Filter> filter, Ptr<Texture> input = nullptr);
    
    static void DisableIrrelvantState();
};
