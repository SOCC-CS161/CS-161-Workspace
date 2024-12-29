#include "test_utils.h"
#include <algorithm>
#include <fstream>
#include <sstream>
#include <regex>
#include <iostream>

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

        return compareOutputs(source_content, test.expected_output, test);
    }
} 