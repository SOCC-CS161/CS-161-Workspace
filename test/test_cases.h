#ifndef TEST_CASES_H
#define TEST_CASES_H

#include <string>
#include <vector>

struct TestCase {
    std::string name;
    std::string input;
    std::string expected_output;
    bool exact_match = false;           // If true, requires exact match. If false, uses contains
    bool ignore_case = false;           // If true, case-insensitive comparison
    bool ignore_whitespace = false;     // If true, ignores extra whitespace
};

std::vector<TestCase> loadTestCases() {
    return {
        {"Contains Test", "42\n", "42", false, false, false},
        {"Exact Match Test", "123\n", "Enter a number: 123\n", true, false, false},
        {"Case Insensitive Test", "42\n", "ENTER A NUMBER: 42\n", true, true, false},
        {"Whitespace Test", "42\n", "42", false, false, true}
    };
}

#endif // TEST_CASES_H 