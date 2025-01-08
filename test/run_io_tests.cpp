#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <cstdio>
#include <algorithm>
#include <iomanip>
#include <numeric>
#include <regex>
#include <unistd.h>  // for access()
#include <ctime>
#include "test_cases.h"
#include "test_utils.h"

// Declare and initialize temp_input_file
std::string temp_input_file = "build/temp_input.txt";

std::string getExpectedTime(const TestCase& test) {
    // Get current UTC time
    time_t now = time(0);
    tm* utc = gmtime(&now);
    
    // Convert to Pacific time (UTC-7 for PDT or UTC-8 for PST)
    utc->tm_hour += test.time_zone_offset;  // -8 for PST, -7 for PDT
    mktime(utc);  // Normalize time in case hour goes negative
    
    // Format time string (without leading zeros)
    std::stringstream ss;
    ss << "Output should contain one of these times: ";
    
    // Store current minute before we start modifying it
    int current_minute = utc->tm_min;
    
    // Show current minute and one minute before/after
    for(int i = -1; i <= 1; i++) {
        utc->tm_min = current_minute + i;
        mktime(utc);  // Normalize in case we crossed hour boundary
        if (i > -1) ss << " or ";
        ss << "'" << utc->tm_hour << ":" << utc->tm_min << "'";
    }
    
    ss << "\nNote: Times can be in H:M format (e.g., 9:5) or HH:MM format (e.g., 09:05)";
    
    return ss.str();
}

void printTestDetails(const TestCase& test, bool passed, const std::string& actual_output, const TestValues& vals) {
    std::cout << "Status: " << (passed ? "✅ PASS" : "❌ FAIL") << "\n";
    std::cout << "Test: " << test.name << "\n\n";
    
    if (test.test_type == TestType::SOURCE) {
        if (test.name == "Part2_Tax_Constant_Usage") {
            std::cout << "Expected: Tax rate (0.06) should be declared as a constant\n";
        } else {
            std::cout << "Expected: " << test.expected_output << "\n";
        }
        std::cout << "Source File: " << test.source_file << "\n";
    } else if (test.comparison_type == ComparisonType::TIME_ACCURACY) {
        std::cout << "Expected: " << getExpectedTime(test) << "\n";
        if (!passed) std::cout << "Actual Output: \"" << actual_output << "\"\n";
    } else if (test.name == "Extra_Credit_Change_Breakdown") {
        // Calculate the change breakdown
        int cents = static_cast<int>(std::round(vals.change_due * 100));
        
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

        std::cout << "Expected: For change of $" << std::fixed << std::setprecision(2) << vals.change_due << ", program should output these quantities in order:\n"
                 << "  " << hundreds << " hundred dollar bills\n"
                 << "  " << fifties << " fifty dollar bills\n"
                 << "  " << twenties << " twenty dollar bills\n"
                 << "  " << ones << " one dollar bills\n"
                 << "  " << quarters << " quarters\n"
                 << "  " << dimes << " dimes\n"
                 << "  " << nickels << " nickels\n"
                 << "  " << pennies << " pennies\n"
                 << "(Note: The exact wording can vary, but the numbers must appear in this order)\n";
        if (!passed) std::cout << "Actual Output: \"" << actual_output << "\"\n";
    } else if (test.expected_output.find("$") == 0) {  // If expected output starts with $
        std::cout << "Expected: Exact string `" << test.expected_output << "` should appear in output\n";
        if (!passed) std::cout << "Actual Output: \"" << actual_output << "\"\n";
    } else {
        std::cout << "Expected: " << test.expected_output << "\n";
        if (!passed) std::cout << "Actual Output: \"" << actual_output << "\"\n";
    }
}

void printSeparator() {
    std::cout << "\n" << std::string(80, '-') << "\n";
}

void runTests(const std::string& program, const std::vector<TestCase>& test_cases, const TestValues& vals) {
    std::cout << "\nRUNNING TESTS\n";
    printSeparator();

    // Only compile if we have runtime tests
    bool needs_compilation = std::any_of(test_cases.begin(), test_cases.end(),
        [](const TestCase& test) { return test.test_type == TestType::RUNTIME; });

    // Compile if needed
    if (needs_compilation) {
        std::string compile_cmd = "mkdir -p build && g++ -std=c++20 -g src/main.cpp -o build/program";
        if (system(compile_cmd.c_str()) != 0) {
            std::cerr << "Error: Failed to compile main.cpp\n";
            return;
        }
    }

    // Initialize full program output
    std::string full_program_output;

    // Write input to a temporary file
    std::ofstream input_file(temp_input_file);
    if (!input_file) {
        std::cerr << "Error: Could not open " << temp_input_file << " for writing.\n";
        return;
    }

    // Write all test inputs to the file
    for (const auto& test : test_cases) {
        if (test.test_type == TestType::RUNTIME && test.has_input) {
            input_file << test.input;
        }
    }
    input_file.close();

    // Capture and display full program output with inputs
    std::cout << "FULL PROGRAM OUTPUT:\n";
    if (access(program.c_str(), X_OK) == 0) {
        std::string command = program + " < " + temp_input_file;
        FILE* pipe = popen(command.c_str(), "r");
        if (pipe) {
            char buffer[128];
            std::vector<std::string> all_inputs;
            for (const auto& test : test_cases) {
                if (test.test_type == TestType::RUNTIME && test.has_input) {
                    std::istringstream iss(test.input);
                    std::string line;
                    while (std::getline(iss, line)) {
                        all_inputs.push_back(line);
                    }
                }
            }
            
            size_t input_idx = 0;
            std::string accumulated_line;
            
            while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
                std::string line(buffer);
                accumulated_line += line;
                
                // Check for each type of prompt
                std::vector<std::string> prompts = {
                    "Enter cost of food: $",
                    "Enter the gratuity percentage: %",
                    "Cash Tendered: $"
                };
                
                for (const auto& prompt : prompts) {
                    size_t pos = accumulated_line.find(prompt);
                    if (pos != std::string::npos) {
                        // Print everything before the prompt
                        if (pos > 0) {
                            std::cout << accumulated_line.substr(0, pos);
                            full_program_output += accumulated_line.substr(0, pos);
                        }
                        
                        // Print the prompt
                        std::cout << prompt;
                        full_program_output += prompt;
                        
                        // Print the input if available
                        if (input_idx < all_inputs.size()) {
                            std::cout << all_inputs[input_idx] << "\n";
                            full_program_output += all_inputs[input_idx] + "\n";
                            input_idx++;
                        }
                        
                        // Keep the rest of the line for next iteration
                        accumulated_line = accumulated_line.substr(pos + prompt.length());
                    }
                }
                
                // If we have a complete line with no more prompts, print it
                if (accumulated_line.find('\n') != std::string::npos && 
                    accumulated_line.find("Enter") == std::string::npos && 
                    accumulated_line.find("Cash Tendered:") == std::string::npos) {
                    std::cout << accumulated_line;
                    full_program_output += accumulated_line;
                    accumulated_line.clear();
                }
            }
            
            // Print any remaining content
            if (!accumulated_line.empty()) {
                std::cout << accumulated_line;
                full_program_output += accumulated_line;
            }
            
            pclose(pipe);
        }
    }
    printSeparator();

    // Run individual tests
    int passed = 0;
    int total = test_cases.size();

    int test_num = 1;
    for (const auto& test : test_cases) {
        std::cout << "Test #" << test_num << ": " << test.name << "\n\n";
        
        bool passed_test;
        std::string actual_output;

        if (test.test_type == TestType::RUNTIME) {
            // Use previously captured full program output
            actual_output = full_program_output;
            passed_test = TestUtils::compareOutputs(actual_output, test.expected_output, test);
            
            // For the test details, we don't want to show the full output
            printTestDetails(test, passed_test, actual_output, vals);
        } else {
            passed_test = TestUtils::runSourceTest(test);
            printTestDetails(test, passed_test, "", vals);
        }
        
        if (passed_test) passed++;
        test_num++;
        printSeparator();
    }

    std::cout << "\nTEST SUMMARY\n";
    std::cout << "Tests Passed: " << passed << "/" << total << " ("
              << std::fixed << std::setprecision(1) << (passed * 100.0 / total) << "%\n";
    printSeparator();
}

int main() {
    std::string program = "./build/program";
    auto test_cases = loadTestCases();
    TestValues vals = TestValues::generate();
    runTests(program, test_cases, vals);
    return 0;
} 