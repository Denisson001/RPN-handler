#pragma once

#include <string>
#include <stack>
#include <vector>

class TSolver {
public:
    void solve();

private:
    struct _TInput {
        std::string expr;
        std::string word;
    };

    struct _TState {
        std::vector< std::vector<bool> > is_reachable;
    };

    std::stack<_TState> _states_stack;
    _TInput             _input;

    void    _readInput();
    void    _printResult();
    void    _error() const;
    void    _solve();
    bool    _pushPlus();
    bool    _pushDot();
    bool    _pushStar();
    void    _pushSymbol(char symbol);
    _TState _createEmptyState() const;
};