//
//  SWTTypes.h
//  TextDetection
//
//  Created by Emiel Bon on 16-07-14.
//  Copyright (c) 2014 Emiel Bon. All rights reserved.
//

#pragma once

// SWT specific
class  Chain;
struct Component;
struct LetterCandidate;
struct Ray;

// Filters
class CannyFilter;
class ConnectedComponentsFilter;
class Filter;
class SWTFilter;

enum class GradientDirection
{
    Unspecified,
    With,
    Against,
};