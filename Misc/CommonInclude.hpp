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
#include <array>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <memory>


inline void AlwaysAssert(bool expr, const char *info){
    if(!expr){
        std::cerr<<info<<"\n";
        throw std::runtime_error{info};
    }
}

#endif //CADA_CPP_IMPLEMENTATIONS_COMMONINCLUDE_HPP
