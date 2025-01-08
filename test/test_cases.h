#ifndef TEST_CASES_H
#define TEST_CASES_H

#include "test_types.h"
#include "test_case_builder.h"
#include <random>
#include <sstream>
#include <iomanip>

// Helper function to generate the change breakdown pattern
inline std::string generateChangePattern(double change) {
    int cents = static_cast<int>(std::round(change * 100));
    
    int hundreds = cents / 10000;
    cents %= 10000;
    int fifties = cents / 5000;
    cents %= 5000;
    int twenties = cents / 2000;
    cents %= 2000;
    int tens = cents / 1000;
    cents %= 1000;
    int fives = cents / 500;
    cents %= 500;
    int ones = cents / 100;
    cents %= 100;
    int quarters = cents / 25;
    cents %= 25;
    int dimes = cents / 10;
    cents %= 10;
    int nickels = cents / 5;
    cents %= 5;
    int pennies = cents;
    
    std::stringstream ss;
    ss << hundreds << ".*" << fifties << ".*" << twenties << ".*" << ones 
       << ".*" << quarters << ".*" << dimes << ".*" << nickels << ".*" << pennies;
    return ss.str();
}

// Function to generate random test values
struct TestValues {
    double food_cost;
    int gratuity_percent;
    double cash_tendered;
    double tax_amount;
    double gratuity_amount;
    double total_due;
    double change_due;
    
    static TestValues generate() {
        TestValues vals;
        
        // Random number generation
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> food_dist(10.0, 50.0);  // Food cost between $10-$50
        std::uniform_int_distribution<> gratuity_dist(10, 20);   // Gratuity between 10-20%
        
        // Generate primary values
        double raw_food = food_dist(gen);
        vals.food_cost = static_cast<int>(raw_food * 100) / 100.0;  // Truncate to 2 decimal places
        vals.gratuity_percent = gratuity_dist(gen);
        
        // Calculate derived values using truncation
        vals.tax_amount = static_cast<int>(vals.food_cost * 0.06 * 100) / 100.0;  // Truncate tax
        vals.gratuity_amount = static_cast<int>(vals.food_cost * vals.gratuity_percent * 0.01 * 100) / 100.0;  // Truncate gratuity
        vals.total_due = vals.food_cost + vals.tax_amount + vals.gratuity_amount;  // Sum of truncated values
        
        // Always give enough cash to ensure positive change
        vals.cash_tendered = std::ceil(vals.total_due / 5.0) * 5.0;  // Round up to next $5
        vals.change_due = vals.cash_tendered - vals.total_due;
        
        return vals;
    }
    
    std::string to_input_string() const {
        std::stringstream ss;
        ss << std::fixed << std::setprecision(2) << food_cost << "\n"
           << gratuity_percent << "\n"
           << cash_tendered << "\n";
        return ss.str();
    }
};

// This file contains all the test cases for the POS system.
// Add new test cases in the loadTestCases() function below.

inline std::vector<TestCase> loadTestCases() {
    // Generate random test values
    TestValues vals = TestValues::generate();
    
    // Format expected values
    std::stringstream food_ss, tax_ss, gratuity_ss, total_ss, change_ss;
    food_ss << std::fixed << std::setprecision(2) << "$" << vals.food_cost;
    tax_ss << std::fixed << std::setprecision(2) << "$" << vals.tax_amount;
    gratuity_ss << std::fixed << std::setprecision(2) << "$" << vals.gratuity_amount;
    total_ss << std::fixed << std::setprecision(2) << "$" << vals.total_due;
    change_ss << std::fixed << std::setprecision(2) << "$" << vals.change_due;

    return {
        // Part 1 - Welcome Message and Time Display
        TestCaseBuilder()
            .name("Part1_Welcome_Message")
            .input(vals.to_input_string())
            .expected_output("Welcome")
            .comparison_type(ComparisonType::CONTAINS)
            .ignore_case(false)
            .ignore_whitespace(true)
            .has_input(false)
            .partial_match(true)
            .test_type(TestType::RUNTIME)
            .source_file("")
            .time_zone_offset(0)
            .accuracy_window(0),

        TestCaseBuilder()
            .name("Part1_Time_Display")
            .input(vals.to_input_string())
            .expected_output("H:M")  // Time format can be H:M or HH:MM
            .comparison_type(ComparisonType::TIME_ACCURACY)
            .ignore_case(false)
            .ignore_whitespace(true)
            .has_input(false)
            .partial_match(true)
            .test_type(TestType::RUNTIME)
            .source_file("")
            .time_zone_offset(-8)
            .accuracy_window(60),

        // Part 2 - Bill Calculation
        TestCaseBuilder()
            .name("Part2_Food_Charge")
            .input(vals.to_input_string())
            .expected_output(food_ss.str())
            .comparison_type(ComparisonType::CONTAINS)
            .ignore_case(false)
            .ignore_whitespace(true)
            .has_input(true)
            .partial_match(true)
            .test_type(TestType::RUNTIME)
            .source_file("")
            .time_zone_offset(0)
            .accuracy_window(0),

        TestCaseBuilder()
            .name("Part2_Tax_Amount")
            .input(vals.to_input_string())
            .expected_output(tax_ss.str())
            .comparison_type(ComparisonType::CONTAINS)
            .ignore_case(false)
            .ignore_whitespace(true)
            .has_input(true)
            .partial_match(true)
            .test_type(TestType::RUNTIME)
            .source_file("")
            .time_zone_offset(0)
            .accuracy_window(0),

        TestCaseBuilder()
            .name("Part2_Gratuity_Amount")
            .input(vals.to_input_string())
            .expected_output(gratuity_ss.str())
            .comparison_type(ComparisonType::CONTAINS)
            .ignore_case(false)
            .ignore_whitespace(true)
            .has_input(true)
            .partial_match(true)
            .test_type(TestType::RUNTIME)
            .source_file("")
            .time_zone_offset(0)
            .accuracy_window(0),

        TestCaseBuilder()
            .name("Part2_Total_Amount")
            .input(vals.to_input_string())
            .expected_output(total_ss.str())
            .comparison_type(ComparisonType::CONTAINS)
            .ignore_case(false)
            .ignore_whitespace(true)
            .has_input(true)
            .partial_match(true)
            .test_type(TestType::RUNTIME)
            .source_file("")
            .time_zone_offset(0)
            .accuracy_window(0),

        TestCaseBuilder()
            .name("Part2_Decimal_Places")
            .input(vals.to_input_string())
            .expected_output("All monetary values should be truncated to no more than two decimal places")
            .comparison_type(ComparisonType::DECIMAL_PRECISION)
            .ignore_case(false)
            .ignore_whitespace(true)
            .has_input(true)
            .partial_match(true)
            .test_type(TestType::RUNTIME)
            .source_file("")
            .time_zone_offset(0)
            .accuracy_window(0),

        TestCaseBuilder()
            .name("Part2_Tax_Constant_Usage")
            .input(vals.to_input_string())
            .expected_output("const.*0\\.0?6")  // Simple regex to find const and 0.06 anywhere on same line
            .comparison_type(ComparisonType::REGEX_MATCH)
            .ignore_case(false)
            .ignore_whitespace(true)
            .has_input(false)
            .partial_match(true)
            .test_type(TestType::SOURCE)
            .source_file("src/main.cpp")
            .time_zone_offset(0)
            .accuracy_window(0)
            .skip_comments(true),

        // Part 3 - Change Calculation
        TestCaseBuilder()
            .name("Part3_Change_Calculation")
            .input(vals.to_input_string())
            .expected_output(change_ss.str())
            .comparison_type(ComparisonType::CONTAINS)
            .ignore_case(false)
            .ignore_whitespace(true)
            .has_input(true)
            .partial_match(true)
            .test_type(TestType::RUNTIME)
            .source_file("")
            .time_zone_offset(0)
            .accuracy_window(0),

        // Extra Credit - Change Breakdown
        TestCaseBuilder()
            .name("Extra_Credit_Change_Breakdown")
            .input(vals.to_input_string())
            .expected_output(generateChangePattern(vals.change_due))
            .comparison_type(ComparisonType::REGEX_MATCH)
            .ignore_case(false)
            .ignore_whitespace(true)
            .has_input(true)
            .partial_match(true)
            .test_type(TestType::RUNTIME)
            .source_file("")
            .time_zone_offset(0)
            .accuracy_window(0)
    };
}

#endif 