//
// Created by xziyu on 2017/9/13.
//

#ifndef CADA_CPP_IMPLEMENTATIONS_COMMONINCLUDE_HPP
#define CADA_CPP_IMPLEMENTATIONS_COMMONINCLUDE_HPP

// STL Libraries
#include <algorithm>
#include <type_traits>
#include <limits>
#include <vector>
#include <string>
#include <list>
#include <map>
#include <set>
#include <iostream>
#include <fstream>
#include <chrono>
#include <random>

using namespace std;

#include "Core"

inline void AlwaysAssert(bool expr, const char *info){
    if(!expr){
        cerr<<info<<"\n";
        throw runtime_error{info};
    }
}

#endif //CADA_CPP_IMPLEMENTATIONS_COMMONINCLUDE_HPP
