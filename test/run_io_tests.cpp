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
#include <unistd.h>  // for access()
#include "test_cases.h"
#include "test_utils.h"

void printTestDetails(const TestCase& test, bool passed, 
                     const std::string& actual_output) {
    std::cout << "Status: " << (passed ? "✅ PASS" : "❌ FAIL") << "\n";
    
    std::string expected_desc;
    if (test.test_type == TestType::SOURCE) {
        if (test.comparison_type == ComparisonType::NOT_CONTAINS) {
            expected_desc = "source code should not contain \"" + test.expected_output + "\"";
        } else {
            expected_desc = "source code should contain \"" + test.expected_output + "\"";
        }
        std::cout << "Test: " << test.name << "\n";
        std::cout << "Expected: " << expected_desc << "\n";
        std::cout << "Source File: " << test.source_file << "\n";
    } else {
        if (test.name == "Output exists and ends with newline") {
            expected_desc = "output ends with newline";
        } else if (test.name == "Output is not 'Hello, Codespaces!'") {
            expected_desc = "any non-empty output except \"Hello, Codespaces!\"";
        }
        std::cout << "Test: " << test.name << "\n";
        std::cout << "Expected: " << expected_desc << "\n";
        std::cout << "Actual Output: \"" << actual_output << "\"\n";
    }
}

void printSeparator() {
    std::cout << "\n" << std::string(80, '-') << "\n";
}

void runTests(const std::string& program, const std::vector<TestCase>& test_cases) {
    std::cout << "\nRUNNING TESTS\n";
    printSeparator();

    // Only compile if we have runtime tests
    bool needs_compilation = std::any_of(test_cases.begin(), test_cases.end(),
        [](const TestCase& test) { return test.test_type == TestType::RUNTIME; });

    if (needs_compilation) {
        std::string compile_cmd = "mkdir -p build && g++ -std=c++20 -g src/main.cpp -o build/program";
        if (system(compile_cmd.c_str()) != 0) {
            std::cerr << "Error: Failed to compile main.cpp\n";
            return;
        }
    }

    int passed = 0;
    int total = test_cases.size();
    
    int test_num = 1;  // Add a separate counter for test numbers
    for (const auto& test : test_cases) {
        std::cout << "Test #" << test_num << ": " << test.name << "\n\n";
        
        bool passed_test;
        std::string actual_output;

        if (test.test_type == TestType::RUNTIME) {
            if (access(program.c_str(), X_OK) != 0) {
                std::cerr << "Error: Program not found or not executable.\n";
                continue;
            }
            
            std::string command = "echo '" + test.input + "' | " + program;
            FILE* pipe = popen(command.c_str(), "r");
            if (!pipe) {
                std::cerr << "Failed to run command\n";
                continue;
            }
            
            char buffer[128];
            while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
                actual_output += buffer;
            }
            pclose(pipe);
            
            passed_test = TestUtils::compareOutputs(actual_output, test.expected_output, test);
        } else {
            // Source code test
            passed_test = TestUtils::runSourceTest(test);
            actual_output = "(source code test)";
        }

        printTestDetails(test, passed_test, actual_output);
        
        if (passed_test) passed++;
        test_num++;
        printSeparator();
    }

    // Print summary
    std::cout << "\nTEST SUMMARY\n";
    std::cout << "Tests Passed: " << passed << "/" << total << " ("
              << std::fixed << std::setprecision(1) << (passed * 100.0 / total) << "%)\n";
    printSeparator();
}

int main() {
    std::string program = "./build/program";
    auto test_cases = loadTestCases();
    runTests(program, test_cases);
    return 0;
} 