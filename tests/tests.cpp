#include "../src/data.h"
#include "../src/solver.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <string>

struct TCfg {
    std::vector<std::string> dirs;
    std::vector<uint32_t>    tests_cnt;
};

TCfg readCfg() {
    std::ifstream cfg_file("tests.cfg");
    TCfg cfg;

    std::string line;
    while (getline(cfg_file, line)) {
        std::stringstream cfg_stream(line);
        std::string header;
        std::string item;
        cfg_stream >> header;
        while(cfg_stream >> item) {
            if (header == "dirs:") {
                cfg.dirs.push_back(item);
            } else { // tests_cnt:
                cfg.tests_cnt.push_back(std::stoi(item));
            }
        }
    }

    cfg_file.close();
    return cfg;
}

std::string createFileName(std::string dir, uint32_t test_num) {
    return dir + "/0" + std::to_string(test_num);
}

std::string runTest(const std::string& test_file) {
    std::ifstream file(test_file);
    TData data;
    file >> data.expr >> data.word;
    file.close();
    TSolver solver;
    solver.setData(&data);
    return solver.solve();
}

std::string readTestResult(const std::string& test_file) {
    std::ifstream file(test_file);
    std::string result;
    file >> result;
    file.close();
    return result;
}

void runTests(const TCfg& cfg) {
    for (uint32_t dir_num = 0; dir_num < cfg.dirs.size(); ++dir_num) {
        const auto& dir = cfg.dirs[dir_num];       
        for (uint32_t test_num = 1; test_num <= cfg.tests_cnt[dir_num]; ++test_num) {
            const auto test_file = createFileName(dir, test_num);
            const auto result = runTest(test_file + ".in");
            const auto correct_result = readTestResult(test_file + ".out");
            const std::string test_result = result == correct_result ? "OK" : "FAIL";
            std::cout << "Test " << test_file << ": " << test_result << "\n";
            std::cout << "Result:         " << result << "\n";
            std::cout << "Correct result: " << correct_result << "\n";
        }
    }
}

int main() {
    const auto cfg = readCfg();
    runTests(cfg);
}
