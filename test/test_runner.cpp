#include "test_runner.h"
#include "test_utils.h"
#include <iostream>
#include <chrono>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cstdio>
#include <unistd.h>
#include <regex>
#include <iomanip>

std::vector<TestResult> runAllTests(const std::string& program, const std::vector<TestCase>& test_cases) {
    std::vector<TestResult> results;
    
    bool needs_compilation = std::any_of(test_cases.begin(), test_cases.end(),
        [](const TestCase& test) { return test.test_type == TestType::RUNTIME; });

    if (needs_compilation) {
        std::string compile_cmd = "mkdir -p build && g++ -std=c++20 -g src/main.cpp -o build/program";
        if (system(compile_cmd.c_str()) != 0) {
            std::cerr << "Error: Failed to compile main.cpp\n";
            return results;
        }
    }

    for (const auto& test : test_cases) {
        auto start_time = std::chrono::high_resolution_clock::now();
        
        TestResult result;
        result.test_name = test.name;
        result.expected = test.expected_output;
        
        if (test.test_type == TestType::RUNTIME) {
            if (access(program.c_str(), X_OK) != 0) {
                std::cerr << "Error: Program not found or not executable.\n";
                continue;
            }
            
            // Get input - either from test case or generate it
            std::string input = test.use_random_input && test.input_generator ? 
                               test.input_generator() : test.input;
            
            // Store the actual input used for reference
            result.input_used = input;
            
            std::string command = "echo '" + input + "' | " + program;
            FILE* pipe = popen(command.c_str(), "r");
            if (!pipe) {
                std::cerr << "Failed to run command\n";
                continue;
            }
            
            std::string actual_output;
            char buffer[128];
            while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
                actual_output += buffer;
            }
            pclose(pipe);
            
            result.actual = actual_output;
            result.passed = TestUtils::compareOutputs(actual_output, test.expected_output, test);
        } else {
            result.passed = TestUtils::runSourceTest(test);
            result.actual = "(source code test)";
        }
        
        auto end_time = std::chrono::high_resolution_clock::now();
        result.duration = std::chrono::duration<double>(end_time - start_time).count();
        
        results.push_back(result);
    }
    
    return results;
}

void printTestResults(const std::vector<TestResult>& results) {
    std::cout << "\nRUNNING TESTS\n";
    std::cout << "\n" << std::string(80, '-') << "\n";

    int passed = 0;
    int test_num = 1;
    
    for (const auto& result : results) {
        std::cout << "Test #" << test_num << ": " << result.test_name << "\n\n";
        std::cout << "Status: " << (result.passed ? "✅ PASS" : "❌ FAIL") << "\n";
        std::cout << "Expected: " << result.expected << "\n";
        std::cout << "Actual: " << result.actual << "\n";
        
        // Print input used if it was a random input test
        if (!result.input_used.empty()) {
            std::cout << "Input Used: " << result.input_used << "\n";
        }
        
        std::cout << "\n" << std::string(80, '-') << "\n";
        
        if (result.passed) passed++;
        test_num++;
    }

    std::cout << "\nTEST SUMMARY\n";
    std::cout << "Tests Passed: " << passed << "/" << results.size() << " ("
              << std::fixed << std::setprecision(1) 
              << (passed * 100.0 / results.size()) << "%)\n";
    std::cout << "\n" << std::string(80, '-') << "\n";
} 