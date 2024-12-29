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
#include <chrono>
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

std::string removeWhitespace(std::string str) {
    str.erase(std::remove_if(str.begin(), str.end(), ::isspace), str.end());
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
        actualStr = removeWhitespace(actualStr);
        expectedStr = removeWhitespace(expectedStr);
    }

    switch (test.comparison_type) {
        case ComparisonType::EQUALS:
            return actualStr == expectedStr;
            
        case ComparisonType::NOT_EQUALS:
            return actualStr != expectedStr && !actualStr.empty();
            
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

bool runSourceTest(const TestCase& test) {
    std::ifstream source_file(test.source_file);
    if (!source_file) {
        std::cerr << "Error: Could not open source file: " << test.source_file << "\n";
        return false;
    }

    std::stringstream buffer;
    buffer << source_file.rdbuf();
    std::string source_content = buffer.str();

    return compareOutputs(source_content, test.expected_output, test);
}

// Add XML helper functions
void writeXmlHeader(std::ofstream& xml) {
    xml << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
        << "<testsuites>\n"
        << "  <testsuite name=\"CS161_Tests\">\n";
}

void writeXmlFooter(std::ofstream& xml) {
    xml << "  </testsuite>\n"
        << "</testsuites>\n";
}

void writeTestResult(std::ofstream& xml, const TestCase& test, bool passed, 
                    const std::string& actual_output, double duration) {
    xml << "    <testcase name=\"" << test.name << "\" "
        << "time=\"" << duration << "\"";
    
    if (!passed) {
        xml << ">\n"
            << "      <failure message=\"Test failed\">\n"
            << "Expected: " << test.expected_output << "\n"
            << "Actual: " << actual_output << "\n"
            << "      </failure>\n"
            << "    </testcase>\n";
    } else {
        xml << "/>\n";
    }
}

void runTests(const std::string& program, const std::vector<TestCase>& test_cases) {
    std::cout << "\nRUNNING TESTS\n";
    printSeparator();

    // Open XML output file
    std::ofstream xml_output("test/test-results.xml");
    writeXmlHeader(xml_output);

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
        
        auto start_time = std::chrono::high_resolution_clock::now();
        
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
            
            passed_test = compareOutputs(actual_output, test.expected_output, test);
        } else {
            // Source code test
            passed_test = runSourceTest(test);
            actual_output = "(source code test)";
        }

        auto end_time = std::chrono::high_resolution_clock::now();
        double duration = std::chrono::duration<double>(end_time - start_time).count();
        
        // Write both console and XML output
        printTestDetails(test, passed_test, actual_output);
        writeTestResult(xml_output, test, passed_test, actual_output, duration);
        
        if (passed_test) passed++;
        test_num++;
        printSeparator();
    }

    // Print summary
    std::cout << "\nTEST SUMMARY\n";
    std::cout << "Tests Passed: " << passed << "/" << total << " ("
              << std::fixed << std::setprecision(1) << (passed * 100.0 / total) << "%)\n";
    printSeparator();
    
    writeXmlFooter(xml_output);
    xml_output.close();
}

int main() {
    std::string program = "./build/program";
    auto test_cases = loadTestCases();
    runTests(program, test_cases);
    return 0;
} 