#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <cstdio>
#include <algorithm>
#include <iomanip>
#include <numeric>
#include <regex>
#include <unistd.h>
#include "test_cases.h"
#include "test_utils.h"
#include "test_runner.h"

int main() {
    const std::string program = "./build/program";
    auto test_cases = loadTestCases();
    
    if (test_cases.empty()) {
        std::cout << "\nNo test cases defined.\n";
        std::cout << "Add test cases to the loadTestCases() function in test/test_cases.h\n";
        std::cout << "See the comments in that file for examples.\n\n";
        return 0;
    }

    auto results = runAllTests(program, test_cases);
    printTestResults(results);
    
    // Return non-zero if any tests failed
    return std::any_of(results.begin(), results.end(), 
                      [](const TestResult& r) { return !r.passed; });
} 