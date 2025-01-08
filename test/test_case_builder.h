#ifndef TEST_CASE_BUILDER_H
#define TEST_CASE_BUILDER_H

#include "test_types.h"

// Helper struct for creating test cases with named parameters
struct TestCaseBuilder {
    TestCase test_case;

    TestCaseBuilder& name(const std::string& value) {
        test_case.name = value;
        return *this;
    }

    TestCaseBuilder& input(const std::string& value) {
        test_case.input = value;
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

    TestCaseBuilder& time_zone_offset(int value) {
        test_case.time_zone_offset = value;
        return *this;
    }

    TestCaseBuilder& accuracy_window(int value) {
        test_case.accuracy_window = value;
        return *this;
    }

    TestCaseBuilder& skip_comments(bool value) {
        test_case.skip_comments = value;
        return *this;
    }

    operator TestCase() const { return test_case; }
};

#endif 