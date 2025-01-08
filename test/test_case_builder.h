#ifndef TEST_CASE_BUILDER_H
#define TEST_CASE_BUILDER_H

#include "test_types.h"

// Helper struct for creating test cases with named parameters
struct TestCaseBuilder {
    TestCase test_case;

    TestCaseBuilder() {
        // Initialize default values
        test_case.use_random_input = false;
        test_case.input_generator = nullptr;
    }

    TestCaseBuilder& name(const std::string& value) {
        test_case.name = value;
        return *this;
    }

    TestCaseBuilder& input(const std::string& value) {
        test_case.input = value;
        test_case.use_random_input = false;  // Disable random input when specific input is set
        return *this;
    }

    TestCaseBuilder& random_input(InputGenerator generator) {
        test_case.input_generator = generator;
        test_case.use_random_input = true;
        return *this;
    }

    TestCaseBuilder& expected_output(const std::string& value) {
        test_case.expected_output = value;
        return *this;
    }

    TestCaseBuilder& comparison_type(ComparisonType value) {
        test_case.comparison_type = value;
        return *this;
    }

    TestCaseBuilder& ignore_case(bool value) {
        test_case.ignore_case = value;
        return *this;
    }

    TestCaseBuilder& ignore_whitespace(bool value) {
        test_case.ignore_whitespace = value;
        return *this;
    }

    TestCaseBuilder& has_input(bool value) {
        test_case.has_input = value;
        return *this;
    }

    TestCaseBuilder& partial_match(bool value) {
        test_case.partial_match = value;
        return *this;
    }

    TestCaseBuilder& test_type(TestType value) {
        test_case.test_type = value;
        return *this;
    }

    TestCaseBuilder& source_file(const std::string& value) {
        test_case.source_file = value;
        return *this;
    }

    TestCaseBuilder& skip_comments(bool value) {
        test_case.skip_comments = value;
        return *this;
    }

    TestCase build() {
        return test_case;
    }
};

#endif 