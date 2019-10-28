#include "solver.h"

#include <iostream>

void TSolver::solve() {
    _solve();
}

void TSolver::_readInput() {
    std::cin >> _input.expr >> _input.word;
}

void TSolver::_error() const {
    std::cout << "ERROR";
}

void TSolver::_printResult() {
    int32_t max_prefix_len = -1;

    uint32_t word_len = _input.word.size();
    for (uint32_t len = word_len; len >= 0; len--) {
        if (_states_stack.top().is_reachable[0][len]) {
            max_prefix_len = len;
            break;
        }
    }

    _states_stack.pop();
    if (max_prefix_len == -1) {
        std::cout << "INF";
    } else {
        std::cout << max_prefix_len;
    }
}

void TSolver::_solve() {
    _readInput();
    bool error = 0;

    for (uint32_t i = 0; i < _input.expr.size() && !error; ++i) {
        const auto current_char = _input.expr[i];
        if (current_char == '+') {
            error = !_pushPlus();
        } else if (current_char == '.') {
            error = !_pushDot();
        } else if (current_char == '*') {
            error = !_pushStar();
        } else { // symbol
            error = current_char != 'a' &&
                    current_char != 'b' &&
                    current_char != 'c' &&
                    current_char != '!';

            if (!error) {
                _pushSymbol(current_char);
            }
        }
    }

    if (_states_stack.size() != 1) {
        error = true;
    }

    if (error) {
        _error();
    }

    _printResult();
}

TSolver::_TState TSolver::_createEmptyState() const {
    _TState new_state;
    uint32_t word_len = _input.word.size();
    new_state.is_reachable.resize(word_len + 1);
    for (uint32_t i = 0; i <= word_len; ++i) {
        new_state.is_reachable[i].resize(word_len + 1, false);
    }
    return new_state;
}

/*
 * inplace merge
 * */
bool TSolver::_pushPlus() {
    if (_states_stack.size() < 2) {
        return false;
    }

    const auto top_state = _states_stack.top();
    _states_stack.pop();

    uint32_t word_len = _input.word.size();
    for (uint32_t i = 0; i <= word_len; ++i) {
        for (uint32_t j = i; j <= word_len; ++j) { // [i..j)
            if (top_state.is_reachable[i][j]) {
                _states_stack.top().is_reachable[i][j] = true;
            }
        }
    }

    return true;
}

bool TSolver::_pushDot() {
    if (_states_stack.size() < 2) {
        return false;
    }

    const auto second_state = _states_stack.top();
    _states_stack.pop();
    const auto first_state = _states_stack.top();
    _states_stack.pop();
    auto new_state = _createEmptyState();

    uint32_t word_len = _input.word.size();
    for (uint32_t i = 0; i <= word_len; ++i) {
        for (uint32_t j = i; j <= word_len; ++j) { // [i..j)
            for (uint32_t k = i; k <= j && !new_state.is_reachable[i][j]; ++k) { // [i..k)[k..j)
                if (first_state.is_reachable[i][k] && second_state.is_reachable[k][j]) {
                    new_state.is_reachable[i][j] = true;
                }
            }
        }
    }

    _states_stack.push(new_state);
    return true;
}

bool TSolver::_pushStar() {
    if (_states_stack.size() < 1) {
        return false;
    }

    const auto top_state = _states_stack.top();
    _states_stack.pop();
    auto new_state = _createEmptyState();

    uint32_t word_len = _input.word.size();
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

void TSolver::_pushSymbol(char symbol) {
    auto new_state = _createEmptyState();

    uint32_t word_len = _input.word.size();
    for (uint32_t i = 0; i <= word_len; ++i) {
        if (symbol == '1') {
            new_state.is_reachable[i][i] = true;
        } else if (i < word_len && _input.word[i] == symbol) {
            new_state.is_reachable[i][i + 1] = true;
        }
    }

    _states_stack.push(new_state);
}