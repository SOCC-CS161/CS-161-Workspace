#ifndef TEST_UTILS_H
#define TEST_UTILS_H

#include <string>
#include "test_cases.h"

namespace TestUtils {
    std::string trim(const std::string& str);
    std::string toLower(std::string str);
    std::string removeWhitespace(std::string str);
    bool compareOutputs(const std::string& actual, const std::string& expected, const TestCase& test);
    bool runSourceTest(const TestCase& test);
}

#endif 