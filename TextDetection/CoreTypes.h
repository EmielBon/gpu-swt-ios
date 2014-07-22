//
//  CoreTypes.h
//  TextDetection
//
//  Created by Emiel Bon on 16-07-14.
//  Copyright (c) 2014 Emiel Bon. All rights reserved.
//

#pragma once

#include <chrono>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>

// Typedefs
using String = std::string;
using StringStream = std::stringstream;
using Exception = std::exception;
using InputFileStream = std::ifstream;
using TimeSpan = std::chrono::high_resolution_clock::duration;