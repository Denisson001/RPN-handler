#pragma once

#include "data.h"

#include <string>
#include <stack>
#include <vector>

class TSolver {
public:
    TSolver();
    std::string solve();
    void        setData(const TData*);

private:
    struct _TState {
        std::vector< std::vector<bool> > is_reachable;
    };

    static const std::string _ERROR;
    static const std::string _INF;

    std::stack<_TState> _states_stack;
    const TData*        _data;

    std::string _calcResult();
    std::string _solve();
    _TState     _createEmptyState() const;
    bool        _pushPlus();
    bool        _pushDot();
    bool        _pushStar();
    void        _pushSymbol(char symbol);
};
