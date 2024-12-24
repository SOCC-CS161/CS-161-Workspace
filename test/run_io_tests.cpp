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
#include "test_cases.h"

// Helper functions for string comparison
std::string trim(const std::string& str) {
    auto start = str.find_first_not_of(" \t\n\r");
    auto end = str.find_last_not_of(" \t\n\r");
    if (start == std::string::npos) return "";
    return str.substr(start, end - start + 1);
}

std::string toLower(std::string str) {
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
    return str;
}

bool compareOutputs(const std::string& actual, const std::string& expected, const TestCase& test) {
    std::string actualStr = actual;
    std::string expectedStr = expected;

    if (test.ignore_case) {
        actualStr = toLower(actualStr);
        expectedStr = toLower(expectedStr);
    }
    if (test.ignore_whitespace) {
        actualStr = trim(actualStr);
        expectedStr = trim(expectedStr);
    }

    if (test.exact_match) {
        return actualStr == expectedStr;
    } else {
        return actualStr.find(expectedStr) != std::string::npos;
    }
}

void printSeparator() {
    std::cout << "\n" << std::string(80, '-') << "\n";
}

void runTests(const std::string& program, const std::vector<TestCase>& test_cases) {
    std::cout << "\nRUNNING TESTS\n";
    printSeparator();

    int passed = 0;
    int total = test_cases.size();
    
    for (const auto& test : test_cases) {
        std::cout << "Test #" << (passed + 1) << ": " << test.name << "\n\n";
        
        // Run the test
        std::string command = "echo '" + test.input + "' | " + program;
        FILE* pipe = popen(command.c_str(), "r");
        if (!pipe) {
            std::cerr << "Failed to run command\n";
            continue;
        }
        
        char buffer[128];
        std::string actual_output;
        while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
            actual_output += buffer;
        }
        pclose(pipe);

        bool passed_test = compareOutputs(actual_output, test.expected_output, test);
        if (passed_test) passed++;

        // Print test details
        std::cout << "Status: " << (passed_test ? "✅ PASS" : "❌ FAIL") << "\n";
        std::cout << "Test Type: " << (test.exact_match ? "Exact Match" : "Contains") << "\n";
        std::cout << "Case Sensitive: " << (!test.ignore_case ? "Yes" : "No") << "\n";
        std::cout << "Whitespace Sensitive: " << (!test.ignore_whitespace ? "Yes" : "No") << "\n";
        std::cout << "Input: \"" << test.input << "\"\n";
        std::cout << "Expected Output: \"" << test.expected_output << "\"\n";
        std::cout << "Actual Output: \"" << actual_output << "\"\n";
        
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