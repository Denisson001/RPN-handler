#include "data.h"

#include <iostream>

/*
 * Чтение инпута
 */
TData readData() {
    TData data;
    std::cin >> data.expr >> data.word;
    return data;
}
