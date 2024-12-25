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

    switch (test.comparison_type) {
        case ComparisonType::EQUALS:
            return actualStr == expectedStr;
            
        case ComparisonType::NOT_EQUALS:
            return actualStr != expectedStr;
            
        case ComparisonType::CONTAINS:
            return actualStr.find(expectedStr) != std::string::npos;
            
        case ComparisonType::NOT_CONTAINS:
            return actualStr.find(expectedStr) == std::string::npos;
            
        case ComparisonType::STARTS_WITH:
            return actualStr.substr(0, expectedStr.length()) == expectedStr;
            
        case ComparisonType::ENDS_WITH:
            return actualStr.length() >= expectedStr.length() &&
                   actualStr.substr(actualStr.length() - expectedStr.length()) == expectedStr;
            
        case ComparisonType::REGEX_MATCH:
            try {
                std::regex pattern(expectedStr);
                return std::regex_search(actualStr, pattern);
            } catch (const std::regex_error&) {
                std::cerr << "Invalid regex pattern: " << expectedStr << "\n";
                return false;
            }
    }
    return false;
}

// Update the test output to show comparison type
void printTestDetails(const TestCase& test, bool passed, 
                     const std::string& actual_output) {
    std::cout << "Status: " << (passed ? "✅ PASS" : "❌ FAIL") << "\n";
    
    std::string comp_type;
    std::string expected_desc;
    switch (test.comparison_type) {
        case ComparisonType::EQUALS: 
            comp_type = "Equals"; 
            expected_desc = "\"" + test.expected_output + "\"";
            break;
        case ComparisonType::NOT_EQUALS: 
            comp_type = "Not Equals"; 
            expected_desc = "not \"" + test.expected_output + "\"";
            break;
        case ComparisonType::CONTAINS: 
            comp_type = "Contains"; 
            expected_desc = "contains \"" + test.expected_output + "\"";
            break;
        case ComparisonType::NOT_CONTAINS: 
            comp_type = "Does Not Contain"; 
            expected_desc = "does not contain \"" + test.expected_output + "\"";
            break;
        case ComparisonType::STARTS_WITH: 
            comp_type = "Starts With"; 
            expected_desc = "starts with \"" + test.expected_output + "\"";
            break;
        case ComparisonType::ENDS_WITH: 
            comp_type = "Ends With"; 
            expected_desc = "ends with \"" + test.expected_output + "\"";
            break;
        case ComparisonType::REGEX_MATCH: 
            comp_type = "Regex Match"; 
            expected_desc = "matches pattern \"" + test.expected_output + "\"";
            break;
    }
    
    std::cout << "Comparison Type: " << comp_type << "\n";
    std::cout << "Case Sensitive: " << (!test.ignore_case ? "Yes" : "No") << "\n";
    std::cout << "Whitespace Sensitive: " << (!test.ignore_whitespace ? "Yes" : "No") << "\n";
    std::cout << "Input: " << (test.has_input ? "\"" + test.input + "\"" : "None") << "\n";
    std::cout << "Expected: " << expected_desc << "\n";
    std::cout << "Actual Output: \"" << actual_output << "\"\n";
}

void printSeparator() {
    std::cout << "\n" << std::string(80, '-') << "\n";
}

void runTests(const std::string& program, const std::vector<TestCase>& test_cases) {
    std::cout << "\nRUNNING TESTS\n";
    printSeparator();

    // Compile the program first
    std::string compile_cmd = "mkdir -p build && g++ -std=c++20 -g src/main.cpp -o build/program";
    if (system(compile_cmd.c_str()) != 0) {
        std::cerr << "Error: Failed to compile main.cpp\n";
        return;
    }

    // Now check if program exists and is executable
    if (access(program.c_str(), X_OK) != 0) {
        std::cerr << "Error: Program '" << program << "' not found or not executable.\n";
        std::cerr << "Something went wrong during compilation.\n";
        return;
    }

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
        printTestDetails(test, passed_test, actual_output);
        
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