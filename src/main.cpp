/*******************************************************************************
 * Author: [Your name here]
 * Last Modified: [Current date]
 * Description: Point of Sale System
 * Input: Total cost of food, gratuity percentage, and amount of cash tendered
 * Output: Prints a greeting to the console, computes taxes, gratuity, and total due, and computes and prints out the change due to the customer
 ******************************************************************************/

#include <iostream>
#include <string>
#include <algorithm>
#include <cctype>

// Global constant for testing source code scanning
const double PI = 3.14159;

// Function to convert string to uppercase
std::string toUpper(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), ::toupper);
    return result;
}

// Function to check if string starts with prefix
bool startsWith(const std::string& str, const std::string& prefix) {
    return str.substr(0, prefix.length()) == prefix;
}

int main() {
    std::string input;
    
    // Welcome message for exact match testing
    std::cout << "Welcome to the Test Framework Demo!" << std::endl;
    
    // Get input for testing
    std::cout << "Enter a string: ";
    std::getline(std::cin, input);
    
    // Output original and uppercase for case sensitivity testing
    std::cout << "You entered: " << input << std::endl;
    std::cout << "Uppercase: " << toUpper(input) << std::endl;
    
    // Output with extra whitespace for whitespace testing
    std::cout << "With    extra    spaces:    " << input << std::endl;
    
    // Conditional output for contains/not contains testing
    if (input.find("hello") != std::string::npos) {
        std::cout << "Input contains 'hello'" << std::endl;
    } else {
        std::cout << "Input does not contain 'hello'" << std::endl;
    }
    
    // Prefix testing
    if (startsWith(input, "test")) {
        std::cout << "Input starts with 'test'" << std::endl;
    }
    
    // Suffix testing
    if (input.length() >= 3 && input.substr(input.length() - 3) == "end") {
        std::cout << "Input ends with 'end'" << std::endl;
    }
    
    // Pattern matching for regex testing
    if (std::all_of(input.begin(), input.end(), ::isdigit)) {
        std::cout << "Pattern: all digits" << std::endl;
    }
    
    return 0;
}