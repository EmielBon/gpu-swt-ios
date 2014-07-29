//
//  types.h
//  OGLTest
//
//  Created by Emiel Bon on 31-01-14.
//  Copyright (c) 2014 Emiel Bon. All rights reserved.
//

#pragma once

// Standard libraries
#include <memory>
#include <functional>
#include <assert.h>

//
#include "ContainerTypes.h"
#include "CoreTypes.h"
#include "FrameworkTypes.h"
#include "GraphicsTypes.h"
#include "Rect.h"
#include "SWTTypes.h"
#include "Vector.h"

// Needed because MacTypes.h is included somewhere and it also defines Ptr as char *
template<class T> using Ptr = std::shared_ptr<T>;
template<class T> using Function = std::function<T>;

// Macros
#define New std::make_shared

#define DEBUG_STEPS

#ifdef DEBUG_STEPS
    #define DEBUG_FB(texture, name) RenderWindow::Instance().AddTexture(texture, name)
    #define assert2(condition, message) if (!(condition)) throw std::runtime_error(String("Error: ") + (message))
    #define crash(message) assert2(false, message)
    #define warning(condition, message) if (!(condition)) printf("%s\n", (message))
#else
    #define DEBUG_FB(texture, name)
    #define assert2(condition, message)
    #define crash(message)
    #define warning(condition, message)
#endif