//
//  ContainerTypes.h
//  TextDetection
//
//  Created by Emiel Bon on 16-07-14.
//  Copyright (c) 2014 Emiel Bon. All rights reserved.
//

#pragma once

#include <list>
#include <map>
#include <set>
#include <stack>
#include <tuple>
#include <vector>

template<class T, class U> using Map = std::map<T, U>;
template<class T> using Set = std::set<T>;
template<class T> using OrderedSet = std::set<T, bool(*)(T, T)>;
template<class T> using List = std::vector<T>;
template<class T> using LinkedList = std::list<T>;
template<class T> using Stack = std::stack<T>;

// Macros
#define Tuple std::tuple