#ifndef TEST_CASES_H
#define TEST_CASES_H

#include <string>
#include <vector>

const std::string NULL_INPUT = "";  // Named constant for no input

enum class ComparisonType {
    EQUALS,             // Exact match
    NOT_EQUALS,         // Must not match exactly
    CONTAINS,           // Must contain substring
    NOT_CONTAINS,       // Must not contain substring
    STARTS_WITH,        // Must start with substring
    ENDS_WITH,         // Must end with substring
    REGEX_MATCH         // Matches regular expression
};

struct TestCase {
    std::string name;
    std::string input;
    std::string expected_output;
    ComparisonType comparison_type = ComparisonType::CONTAINS;
    bool ignore_case = false;
    bool ignore_whitespace = false;
    bool has_input = true;
};

std::vector<TestCase> loadTestCases() {
    return {
        {
            "Must contain 'hello'", 
            NULL_INPUT,
            "hello",
            ComparisonType::CONTAINS,
            true,   // case insensitive
            true,   // ignore whitespace
            false   // no input
        },
        {
            "Must not contain 'world'",
            NULL_INPUT,
            "world",
            ComparisonType::NOT_CONTAINS,
            true,   // case insensitive
            true,   // ignore whitespace
            false   // no input
        }
    };
}

#endif // TEST_CASES_H 