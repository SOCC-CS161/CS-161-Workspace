#include "test_utils.h"
#include "test_cases.h"
#include <algorithm>
#include <fstream>
#include <sstream>
#include <regex>
#include <iostream>
#include <random>

// Implementation of random input generator
std::string generateRandomDigits() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> len_dist(1, 10);  // Random length between 1 and 10
    std::uniform_int_distribution<> digit_dist(0, 9);  // Random digits

    int length = len_dist(gen);
    std::string result;
    for (int i = 0; i < length; ++i) {
        result += std::to_string(digit_dist(gen));
    }
    result += "\n";  // Add newline for input
    return result;
}

namespace TestUtils {
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

    std::string removeComments(const std::string& source) {
        std::string result;
        bool in_line_comment = false;
        bool in_block_comment = false;
        
        for (size_t i = 0; i < source.length(); ++i) {
            if (in_line_comment) {
                if (source[i] == '\n') {
                    in_line_comment = false;
                    result += '\n';
                }
                continue;
            }
            
            if (in_block_comment) {
                if (i + 1 < source.length() && source[i] == '*' && source[i + 1] == '/') {
                    in_block_comment = false;
                    ++i;
                }
                continue;
            }
            
            if (i + 1 < source.length()) {
                if (source[i] == '/' && source[i + 1] == '/') {
                    in_line_comment = true;
                    ++i;
                    continue;
                }
                if (source[i] == '/' && source[i + 1] == '*') {
                    in_block_comment = true;
                    ++i;
                    continue;
                }
            }
            
            result += source[i];
        }
        
        return result;
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

    bool runSourceTest(const TestCase& test) {
        std::ifstream source_file(test.source_file);
        if (!source_file) {
            std::cerr << "Error: Could not open source file: " << test.source_file << "\n";
            return false;
        }

        std::stringstream buffer;
        buffer << source_file.rdbuf();
        std::string source_content = buffer.str();

        // Remove comments if requested
        if (test.skip_comments) {
            source_content = removeComments(source_content);
        }

        return compareOutputs(source_content, test.expected_output, test);
    }
} 