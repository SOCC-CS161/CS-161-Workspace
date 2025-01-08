#ifndef TEST_TYPES_H
#define TEST_TYPES_H

#include <string>
#include <vector>

enum class ComparisonType {
    EQUALS,             // Exact match
    NOT_EQUALS,         // Must not match exactly
    CONTAINS,           // Must contain substring
    NOT_CONTAINS,       // Must not contain substring
    STARTS_WITH,        // Must start with substring
    ENDS_WITH,         // Must end with substring
    REGEX_MATCH,       // Matches regular expression
    TIME_ACCURACY,      // Special comparison for time accuracy
    DECIMAL_PRECISION   // Check for exactly 2 decimal places
};

enum class TestType {
    RUNTIME,    // Tests program output
    SOURCE      // Tests source code content
};

struct TestCase {
    std::string name;
    std::string input;
    std::string expected_output;
    ComparisonType comparison_type;
    bool ignore_case;
    bool ignore_whitespace;
    bool has_input;
    bool partial_match;
    TestType test_type;
    std::string source_file;
    int time_zone_offset;
    int accuracy_window;
    bool skip_comments;
};

#endif 