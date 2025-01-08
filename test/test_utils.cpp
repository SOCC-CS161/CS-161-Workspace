#include "test_utils.h"
#include <algorithm>
#include <fstream>
#include <sstream>
#include <regex>
#include <iostream>
#include <iomanip>

namespace TestUtils {
    std::string trim(const std::string& str) {
        auto start = str.find_first_not_of(" \t\n\r");
        auto end = str.find_last_not_of(" \t\n\r");
        if (start == std::string::npos) return "";
        return str.substr(start, end - start + 1);
    }

    std::string removeComments(const std::string& source) {
        std::string result;
        bool in_line_comment = false;
        bool in_block_comment = false;
        bool in_string = false;
        char prev_char = '\0';

        for (size_t i = 0; i < source.length(); ++i) {
            char c = source[i];
            char next_char = (i + 1 < source.length()) ? source[i + 1] : '\0';

            if (in_string) {
                result += c;
                if (c == '\"' && prev_char != '\\') {
                    in_string = false;
                }
            } else if (in_line_comment) {
                if (c == '\n') {
                    result += '\n';
                    in_line_comment = false;
                }
            } else if (in_block_comment) {
                if (c == '*' && next_char == '/') {
                    in_block_comment = false;
                    ++i;  // Skip the '/'
                }
            } else {
                if (c == '\"') {
                    in_string = true;
                    result += c;
                } else if (c == '/' && next_char == '/') {
                    in_line_comment = true;
                    ++i;  // Skip the second '/'
                } else if (c == '/' && next_char == '*') {
                    in_block_comment = true;
                    ++i;  // Skip the '*'
                } else {
                    result += c;
                }
            }
            prev_char = c;
        }
        return result;
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

        // Special handling for monetary values (starting with $)
        if (expected.find("$") == 0) {
            // Extract the number without the $ sign
            std::string number = expected.substr(1);  // Remove $
            // Create pattern that matches with or without trailing zero
            std::string pattern = "\\$" + number;
            if (pattern.back() == '0' && pattern[pattern.length()-2] == '.') {
                // If ends in .0, make the 0 optional
                pattern = pattern.substr(0, pattern.length()-1) + "0?";
            }
            std::regex money_pattern(pattern);
            return std::regex_search(actualStr, money_pattern);
        }

        // Only modify case/whitespace for non-decimal precision checks
        if (test.comparison_type != ComparisonType::DECIMAL_PRECISION) {
            if (test.ignore_case) {
                actualStr = toLower(actualStr);
                expectedStr = toLower(expectedStr);
            }

            if (test.ignore_whitespace) {
                actualStr = removeWhitespace(actualStr);
                expectedStr = removeWhitespace(expectedStr);
            }
        }

        switch (test.comparison_type) {
            case ComparisonType::TIME_ACCURACY: {
                // Get current time in H:M format (allowing single digits)
                time_t now = time(0);
                tm* utc = gmtime(&now);
                utc->tm_hour += test.time_zone_offset;
                mktime(utc);  // Normalize time
                
                // Create both formats of the time (with and without leading zeros)
                std::stringstream ss1, ss2;
                ss1 << utc->tm_hour << ":" << utc->tm_min;  // e.g. "8:5"
                ss2 << std::setfill('0') << std::setw(2) << utc->tm_hour << ":" 
                    << std::setfill('0') << std::setw(2) << utc->tm_min;  // e.g. "08:05"
                
                std::cout << "Checking if output contains either '" << ss1.str() << "' or '" << ss2.str() << "'\n";
                return actualStr.find(ss1.str()) != std::string::npos || 
                       actualStr.find(ss2.str()) != std::string::npos;
            }
            case ComparisonType::DECIMAL_PRECISION: {
                // Look for any numbers with more than 2 decimal places
                std::regex decimal_pattern("\\$?\\d+\\.\\d{3,}");
                std::smatch match;
                if (std::regex_search(actualStr, match, decimal_pattern)) {
                    // Get some context around the match
                    size_t start = match.position();
                    size_t context_start = start > 50 ? start - 50 : 0;
                    size_t context_length = 100;
                    std::string context = actualStr.substr(context_start, context_length);
                    
                    std::cout << "Found value with too many decimal places: " << match[0] << "\n";
                    std::cout << "Context (with line breaks preserved):\n" << context << "\n";
                    return false;
                }
                return true;
            }
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

        if (test.test_type == TestType::SOURCE) {
            std::string expected_desc;
            if (test.comparison_type == ComparisonType::NOT_CONTAINS) {
                expected_desc = "source code should not contain " + test.expected_output;
            } else {
                expected_desc = test.expected_output;
            }
            std::cout << "Expected: " << expected_desc << "\n";
            std::cout << "Source File: " << test.source_file << "\n";
        }

        return compareOutputs(source_content, test.expected_output, test);
    }
} 