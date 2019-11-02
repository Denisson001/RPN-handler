#include "solver.h"

const std::string    TSolver::_ERROR         = "ERROR";
const std::string    TSolver::_INF           = "INF";
const std::set<char> TSolver::_VALID_SYMBOLS = {'a', 'b', 'c', '1'};

TSolver::TSolver():
    _data(nullptr)
{}

/*
 * Установить указатель на инпут.
 * */
void TSolver::setData(const TData* data) {
    _data = data;
}

/*
 * Точка входа в решение.
 * */
std::string TSolver::solve() {
    if (_data == nullptr) {
        return _ERROR;
    }
    return _solve();
}

/*
 * Возвращает максимальную длину префикса, лежащего в языке.
 * Если никакой префикс не лежит в языке, возвращает INF.
 * */
std::string TSolver::_calcResult() {
    int32_t max_prefix_len = -1;

    uint32_t word_len = _data->word.size();
    for (int32_t len = word_len; len >= 0; len--) {
        if (_states_stack.top().is_reachable[0][len]) {
            max_prefix_len = len;
            break;
        }
    }

    _states_stack.pop();
    if (max_prefix_len == -1) {
        return _INF;    
    }

    return std::to_string(max_prefix_len);   
}

/*
 * Итеративно обрабатывает рег. выражение в обратной польской нотации.
 * Размер стека после обработки должен быть равен 1.
 * Возвращает ERROR или результат вызова _calcResult().
 * */
std::string TSolver::_solve() {
    bool error = 0;

    for (uint32_t i = 0; i < _data->expr.size() && !error; ++i) {
        const auto current_char = _data->expr[i];
        if (current_char == '+') {
            error = !_pushPlus();
        } else if (current_char == '.') {
            error = !_pushDot();
        } else if (current_char == '*') {
            error = !_pushStar();
        } else { // symbol
            if (_VALID_SYMBOLS.count(current_char)) {
                _pushSymbol(current_char);
            } else {
                error = true;
            }
        }
    }

    if (_states_stack.size() != 1) {
        error = true;
    }

    if (error) {
        return _ERROR;
    }

    return _calcResult();
}


/*
 * Создает и возвращает матрицу _TState размера (word_len + 1) x (word_len + 1), заполненную false.
 * */
TSolver::_TState TSolver::_createEmptyState() const {
    _TState new_state;
    uint32_t word_len = _data->word.size();
    new_state.is_reachable.resize(word_len + 1);
    for (uint32_t i = 0; i <= word_len; ++i) {
        new_state.is_reachable[i].resize(word_len + 1, false);
    }
    return new_state;
}

/*
 * Обработка '+' рег. выражения.
 * Метод делает поэлементное логическое 'или' значений матриц динамического программирования двух верхних элементов стека.
 * Inplace merge.
 * Возвращает false, если элементов на стеке меньше 2.
 * */
bool TSolver::_pushPlus() {
    if (_states_stack.size() < 2) {
        return false;
    }

    const auto top_state = _states_stack.top();
    _states_stack.pop();

    uint32_t word_len = _data->word.size();
    for (uint32_t i = 0; i <= word_len; ++i) {
        for (uint32_t j = i; j <= word_len; ++j) { // [i..j)
            if (top_state.is_reachable[i][j]) {
                _states_stack.top().is_reachable[i][j] = true;
            }
        }
    }

    return true;
}

/*
 * Обработка '.' рег. выражения.
 * Метод снимает две верхних матрицы со стека.
 * Новое значение true в позиции i, j <=> существует k, что значения в старых матрицах
 * на позициях i, k и k, j соответственно были true.
 * Добавляет новую матрицу на стек.
 * Возвращает false, если элементов на стеке меньше 2.
 * */
bool TSolver::_pushDot() {
    if (_states_stack.size() < 2) {
        return false;
    }

    const auto second_state = _states_stack.top();
    _states_stack.pop();
    const auto first_state  = _states_stack.top();
    _states_stack.pop();
    auto new_state = _createEmptyState();

    uint32_t word_len = _data->word.size();
    for (uint32_t i = 0; i <= word_len; ++i) {
        for (uint32_t j = i; j <= word_len; ++j) { // [i..j)
            for (uint32_t k = i; k <= j; ++k) { // [i..k)[k..j)
                if (first_state.is_reachable[i][k] && second_state.is_reachable[k][j]) {
                    new_state.is_reachable[i][j] = true;
                    break;
                }
            }
        }
    }

    _states_stack.push(new_state);
    return true;
}

/*
 * Обработка '*' рег. выражения.
 * Метод снимает верхнюю матрицу со стека.
 * Новое значение true в позиции i, j <=> этот интервал [i, j) можно представить в виде последовательности
 * непересекающихся интервалов, покрывающих интервал [i, j), значения для которых true в матрице с вершины стека.
 * Значения [i, i) становятся true.
 * Для пересчета удобно использовать уже посчитанные новые значения.
 * Добавляет новую матрицу на стек.
 * Возвращает false, если стек пустой.
 * */
bool TSolver::_pushStar() {
    if (_states_stack.size() < 1) {
        return false;
    }

    const auto top_state = _states_stack.top();
    _states_stack.pop();
    auto new_state = _createEmptyState();

    uint32_t word_len = _data->word.size();
    for (uint32_t i = 0; i <= word_len; ++i) {
        new_state.is_reachable[i][i] = true;
        for (uint32_t j = i + 1; j <= word_len; ++j) { // [i..j)
            for (uint32_t k = i; k <= j && !new_state.is_reachable[i][j]; ++k) { // [i..k)[k..j)
                if (new_state.is_reachable[i][k] && top_state.is_reachable[k][j]) {
                    new_state.is_reachable[i][j] = true;
                }
            }
        }
    }

    _states_stack.push(new_state);
    return true;
}

/*
 * Обработка 'a', 'b', 'c' или '1' рег. выражения.
 * Если '1', то значения [i, i) проставляет true
 * Если 'a', 'b' или 'c', то проставляет значения [i, i + 1) true, если соответствующие символы совпадают
 * Добавляет новую матрицу на стек.
 * */
void TSolver::_pushSymbol(char symbol) {
    auto new_state = _createEmptyState();

    uint32_t word_len = _data->word.size();
    for (uint32_t i = 0; i <= word_len; ++i) {
        if (symbol == '1') {
            new_state.is_reachable[i][i] = true;
        } else if (i < word_len && _data->word[i] == symbol) { // symbol = 'a', 'b' or 'c'
            new_state.is_reachable[i][i + 1] = true;
        }
    }

    _states_stack.push(new_state);
}
