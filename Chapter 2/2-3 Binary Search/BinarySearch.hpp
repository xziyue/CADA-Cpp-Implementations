//
// Created by xziyu on 2017/9/13.
//

#ifndef CADA_CPP_IMPLEMENTATIONS_BINARYSEARCH_HPP
#define CADA_CPP_IMPLEMENTATIONS_BINARYSEARCH_HPP

#include "CommonInclude.hpp"

// Pass in iterators and search target
// searches the target in [left,right), returns right if the target is not found
template<typename IteratorType, typename ValueType,typename Compare>
IteratorType BinarySearch(IteratorType left, IteratorType right, const ValueType &target, Compare compareLess) {
    auto end = right--;
    while (left <= right) {
        auto dist = distance(left,right);
        auto middle = left + (dist / 2);
        if (target == *middle)return middle;
        if (!compareLess(target,*middle))left = middle + 1;
        else right = middle - 1;
    }
    return end;
}

template<typename IteratorType, typename ValueType>
IteratorType BinarySearch(IteratorType left, IteratorType right, const ValueType &target) {
    return BinarySearch(left,right,target,less<ValueType>{});
}


#endif //CADA_CPP_IMPLEMENTATIONS_BINARYSEARCH_HPP
