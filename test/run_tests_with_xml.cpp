#include "test_runner.h"
#include <fstream>

void writeXmlResults(const std::vector<TestResult>& results, const std::string& filename) {
    std::ofstream xml(filename);
    
    xml << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
        << "<testsuites>\n"
        << "  <testsuite name=\"CS161_Tests\">\n";
    
    for (const auto& result : results) {
        xml << "    <testcase name=\"" << result.test_name << "\" "
            << "time=\"" << result.duration << "\"";
        
        if (!result.passed) {
            xml << ">\n"
                << "      <failure message=\"Test failed\">\n"
                << "Expected: " << result.expected << "\n"
                << "Actual: " << result.actual << "\n"
                << "      </failure>\n"
                << "    </testcase>\n";
        } else {
            xml << "/>\n";
        }
    }
    
    xml << "  </testsuite>\n"
        << "</testsuites>\n";
}

int main() {
    auto test_cases = loadTestCases();
    auto results = runAllTests("./build/program", test_cases);
    
    // Write both outputs
    printTestResults(results);
    writeXmlResults(results, "test/test-results.xml");
    return 0;
} 