#include "solver.h"
#include "data.h"

#include <iostream>

TData readData();

int main() {
    const auto data = readData();
    TSolver solver;
    solver.setData(&data);
    const auto result = solver.solve();
    std::cout << result;
}
