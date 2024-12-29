#ifndef TEST_CASES_H
#define TEST_CASES_H

#include <string>
#include <vector>

enum class ComparisonType {
    EQUALS,             // Exact match
    NOT_EQUALS,         // Must not match exactly
    CONTAINS,           // Must contain substring
    NOT_CONTAINS,       // Must not contain substring
    STARTS_WITH,        // Must start with substring
    ENDS_WITH,         // Must end with substring
    REGEX_MATCH         // Matches regular expression
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
    std::string source_file;  // Only used for SOURCE tests
};

inline std::vector<TestCase> loadTestCases() {
    return {
        {
            "Output exists and ends with newline",
            "", // no input needed
            "\n", // just checking for newline at end
            ComparisonType::ENDS_WITH,
            false, // case sensitive
            false, // don't ignore whitespace
            false, // no input
            true,  // partial match
            TestType::RUNTIME,
            ""
        },
        {
            "Output is not 'Hello, Codespaces!'",
            "", // no input needed
            "Hello, Codespaces!\n",
            ComparisonType::NOT_EQUALS,
            false, // case sensitive
            false, // don't ignore whitespace
            false, // no input
            false, // exact match
            TestType::RUNTIME,
            ""
        },
        {
            "Author name has been added",
            "", // no input needed
            "[Your name here]", // text that should NOT be present
            ComparisonType::NOT_CONTAINS,
            true,  // case insensitive
            true,  // ignore whitespace
            false, // no input
            false, // exact match
            TestType::SOURCE,
            "src/main.cpp"
        }
    };
}

#endif 