#ifndef TEST_RUNNER_H
#define TEST_RUNNER_H

#include <vector>
#include <string>
#include "test_cases.h"

struct TestResult {
    std::string test_name;
    std::string expected;
    std::string actual;
    bool passed;
    double duration;
    std::string input_used;
};

std::vector<TestResult> runAllTests(const std::string& program, const std::vector<TestCase>& test_cases);
void printTestResults(const std::vector<TestResult>& results);

#endif 