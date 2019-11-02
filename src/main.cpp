#include "solver.h"
#include "data.h"

#include <iostream>

TData readData();

int main() {
    /*
     * Чтение инпута.
     * */
    const auto data = readData();
    /*
     * Решение задачи.
     * */
    TSolver solver;
    solver.setData(&data);
    const auto result = solver.solve();
    /*
     * Вывод результата.
     * */
    std::cout << result;
}
