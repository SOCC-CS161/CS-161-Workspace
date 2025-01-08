#ifndef TEST_CASES_H
#define TEST_CASES_H

#include "test_types.h"
#include "test_case_builder.h"
#include <random>

// Helper function to generate random digits
std::string generateRandomDigits();

// Example test cases demonstrating all features of the test framework
inline std::vector<TestCase> loadTestCases() {
    return {
        // 1. Basic Runtime Test - Exact Match
        TestCaseBuilder()
            .name("Welcome Message - Exact Match")
            .input("")
            .expected_output("Welcome to the Test Framework Demo!")
            .comparison_type(ComparisonType::CONTAINS)
            .ignore_case(false)
            .ignore_whitespace(true)
            .has_input(false)
            .partial_match(true)
            .test_type(TestType::RUNTIME)
            .source_file("")
            .build(),

        // 2. Case Insensitive Test
        TestCaseBuilder()
            .name("Case Insensitive Matching")
            .input("Hello World\n")
            .expected_output("HELLO WORLD")
            .comparison_type(ComparisonType::CONTAINS)
            .ignore_case(true)
            .ignore_whitespace(false)
            .has_input(true)
            .partial_match(true)
            .test_type(TestType::RUNTIME)
            .source_file("")
            .build(),

        // 3. Whitespace Ignoring Test
        TestCaseBuilder()
            .name("Whitespace Ignoring")
            .input("test string\n")
            .expected_output("With extra spaces: test string")
            .comparison_type(ComparisonType::CONTAINS)
            .ignore_case(false)
            .ignore_whitespace(true)
            .has_input(true)
            .partial_match(true)
            .test_type(TestType::RUNTIME)
            .source_file("")
            .build(),

        // 4. Contains Test
        TestCaseBuilder()
            .name("Contains String")
            .input("hello world\n")
            .expected_output("Input contains 'hello'")
            .comparison_type(ComparisonType::CONTAINS)
            .ignore_case(false)
            .ignore_whitespace(false)
            .has_input(true)
            .partial_match(true)
            .test_type(TestType::RUNTIME)
            .source_file("")
            .build(),

        // 5. Not Contains Test
        TestCaseBuilder()
            .name("Not Contains String")
            .input("test string\n")
            .expected_output("Input does not contain 'hello'")
            .comparison_type(ComparisonType::CONTAINS)
            .ignore_case(false)
            .ignore_whitespace(false)
            .has_input(true)
            .partial_match(true)
            .test_type(TestType::RUNTIME)
            .source_file("")
            .build(),

        // 6. Starts With Test
        TestCaseBuilder()
            .name("Starts With")
            .input("test case\n")
            .expected_output("Input starts with 'test'")
            .comparison_type(ComparisonType::CONTAINS)
            .ignore_case(false)
            .ignore_whitespace(false)
            .has_input(true)
            .partial_match(true)
            .test_type(TestType::RUNTIME)
            .source_file("")
            .build(),

        // 7. Ends With Test
        TestCaseBuilder()
            .name("Ends With")
            .input("this is the end\n")
            .expected_output("Input ends with 'end'")
            .comparison_type(ComparisonType::CONTAINS)
            .ignore_case(false)
            .ignore_whitespace(false)
            .has_input(true)
            .partial_match(true)
            .test_type(TestType::RUNTIME)
            .source_file("")
            .build(),

        // 8. Regex Pattern Test
        TestCaseBuilder()
            .name("Regex Pattern")
            .input("12345\n")
            .expected_output("Pattern: all digits")
            .comparison_type(ComparisonType::CONTAINS)
            .ignore_case(false)
            .ignore_whitespace(false)
            .has_input(true)
            .partial_match(true)
            .test_type(TestType::RUNTIME)
            .source_file("")
            .build(),

        // 9. Source Code Test - With Comments
        TestCaseBuilder()
            .name("Source Code Constant - Skip Comments")
            .input("")
            .expected_output("const.*PI.*=.*3.14159")
            .comparison_type(ComparisonType::REGEX_MATCH)
            .ignore_case(false)
            .ignore_whitespace(true)
            .has_input(false)
            .partial_match(true)
            .test_type(TestType::SOURCE)
            .source_file("src/main.cpp")
            .skip_comments(true)
            .build(),

        // 10. Not Equals Test
        TestCaseBuilder()
            .name("Not Equals Test")
            .input("")
            .expected_output("Goodbye World!")
            .comparison_type(ComparisonType::NOT_EQUALS)
            .ignore_case(false)
            .ignore_whitespace(false)
            .has_input(false)
            .partial_match(false)
            .test_type(TestType::RUNTIME)
            .source_file("")
            .build(),

        // 11. Source Test - Find String in Comments
        TestCaseBuilder()
            .name("Source Test - Find String in Comments")
            .input("")
            .expected_output("Your name here")  // This text is in a comment in main.cpp
            .comparison_type(ComparisonType::CONTAINS)
            .ignore_case(false)
            .ignore_whitespace(true)
            .has_input(false)
            .partial_match(true)
            .test_type(TestType::SOURCE)
            .source_file("src/main.cpp")
            .skip_comments(false)  // Don't skip comments
            .build(),

        // 12. Source Test - Skip Comments
        TestCaseBuilder()
            .name("Source Test - Skip Comments")
            .input("")
            .expected_output("Your name here")  // This text should not be found when skipping comments
            .comparison_type(ComparisonType::NOT_CONTAINS)  // Should NOT find the text when comments are skipped
            .ignore_case(false)
            .ignore_whitespace(true)
            .has_input(false)
            .partial_match(true)
            .test_type(TestType::SOURCE)
            .source_file("src/main.cpp")
            .skip_comments(true)  // Skip comments
            .build(),

        // 13. Random Input Test
        TestCaseBuilder()
            .name("Random Digits Test")
            .random_input(generateRandomDigits)  // Use random input generator
            .expected_output("Pattern: all digits")
            .comparison_type(ComparisonType::CONTAINS)
            .ignore_case(false)
            .ignore_whitespace(false)
            .has_input(true)
            .partial_match(true)
            .test_type(TestType::RUNTIME)
            .source_file("")
            .build()
    };
}

#endif 