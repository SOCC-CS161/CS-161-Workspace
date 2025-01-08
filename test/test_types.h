#ifndef TEST_TYPES_H
#define TEST_TYPES_H

#include <string>
#include <vector>
#include <functional>

enum class ComparisonType {
    EQUALS,             // Exact match
    NOT_EQUALS,         // Must not match exactly
    CONTAINS,           // Must contain substring
    NOT_CONTAINS,       // Must not contain substring
    STARTS_WITH,        // Must start with substring
    ENDS_WITH,         // Must end with substring
    REGEX_MATCH        // Matches regular expression
};

enum class TestType {
    RUNTIME,    // Tests program output
    SOURCE      // Tests source code content
};

// Function type for generating random input
using InputGenerator = std::function<std::string()>;

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
    bool skip_comments;    // For source tests: whether to ignore comments when searching
    InputGenerator input_generator;  // Optional function to generate random input
    bool use_random_input;          // Whether to use random input generation
};

#endif 