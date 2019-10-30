#include "data.h"

#include <iostream>

TData readData() {
    TData data;
    std::cin >> data.expr >> data.word;
    return data;
}
