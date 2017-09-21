//
// Created by xziyu on 2017/9/13.
//
#include "BinarySearch.hpp"

const char *invalidSearchResultString = "the search result is in correct";

int main() {
    // create a vector with 21 consecutive even numbers
    std::vector<int> testArray;
    for (auto i = 0; i < 21; ++i)testArray.push_back(2 * i);

    // find random even numbers in the test array
    std::default_random_engine randomEigine;
    std::mt19937 generator{randomEigine()};
    std::uniform_int_distribution<> distribution{0, 60};

    // test vector iterators
    for (auto i = 0; i < 200; ++i) {
        auto target = distribution(generator);
        auto result = BinarySearch(testArray.begin(), testArray.end(), target);
        if (target % 2 != 0 || target > 40)AlwaysAssert(result == testArray.end(), invalidSearchResultString);
        else AlwaysAssert(*result == target, invalidSearchResultString);
    }

    // test plain pointers
    for (auto i = 0; i < 200; ++i) {
        auto target = distribution(generator);
        auto result = BinarySearch(testArray.data(), testArray.data() + testArray.size(), target);
        if (target % 2 != 0 || target > 40)AlwaysAssert(result == testArray.data() + testArray.size(), invalidSearchResultString);
        else AlwaysAssert(*result == target, invalidSearchResultString);
    }

    std::cout << "test finished\n";
    return 0;
}