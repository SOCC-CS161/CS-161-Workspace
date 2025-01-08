#ifndef TEST_CASES_H
#define TEST_CASES_H

#include "test_types.h"
#include "test_case_builder.h"

// Example test cases demonstrating all features of the test framework
inline std::vector<TestCase> loadTestCases() {
    return {
        TestCaseBuilder()
            .name("Hello World Test")
            .input("")
            .expected_output("Hello, World!\n")
            .comparison_type(ComparisonType::EQUALS)
            .ignore_case(false)
            .ignore_whitespace(false)
            .has_input(false)
            .partial_match(false)
            .test_type(TestType::RUNTIME)
            .source_file("")
            .build()
    };
}

/*
Example test cases demonstrating framework features:

1. Basic Runtime Test (Exact Match):
    TestCaseBuilder()
        .name("Example - Exact Match")
        .input("")
        .expected_output("Hello, World!")
        .comparison_type(ComparisonType::EQUALS)
        .ignore_case(false)
        .ignore_whitespace(false)
        .has_input(false)
        .partial_match(false)
        .test_type(TestType::RUNTIME)
        .source_file("")
        .build();

2. Case Insensitive Test:
    TestCaseBuilder()
        .name("Example - Case Insensitive")
        .input("HELLO\n")
        .expected_output("hello")
        .comparison_type(ComparisonType::CONTAINS)
        .ignore_case(true)
        .ignore_whitespace(false)
        .has_input(true)
        .partial_match(true)
        .test_type(TestType::RUNTIME)
        .source_file("")
        .build();

3. Source Code Test:
    TestCaseBuilder()
        .name("Example - Source Code")
        .input("")
        .expected_output("TODO")
        .comparison_type(ComparisonType::CONTAINS)
        .ignore_case(false)
        .ignore_whitespace(true)
        .has_input(false)
        .partial_match(true)
        .test_type(TestType::SOURCE)
        .source_file("src/main.cpp")
        .skip_comments(true)
        .build();

Other features demonstrated in the examples above:
- EQUALS, CONTAINS, NOT_CONTAINS comparison types
- STARTS_WITH, ENDS_WITH for prefix/suffix matching
- REGEX_MATCH for pattern matching
- Whitespace sensitivity control
- Case sensitivity control
- Source code comment handling
*/

#endif 