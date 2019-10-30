#pragma once

#include "data.h"

#include <string>
#include <stack>
#include <vector>

/* 
 * Класс - решение задачи.
 */
class TSolver {
public:
    TSolver();
    
    /*
     * Public-функция. Решает задачу для данных в _data.
     * Может вернуть: 
     * Число - ответ на задачу;
     * ERROR - некорректное рег. выражение;
     * INF   - никакой префикс слова, даже пустой, не лежит в языке.
     */
    std::string solve();

    /*
     * Public-функция. Устанавливает _data.
     */
    void setData(const TData*);

private:
    /*
     * Структура - состояние динамического программирования для подвыражения исходного рег. выражения.
     * Содержит в себе битовую матрицу is_reachable[WORD_LEN + 1][WORD_LEN + 1].
     * is_reachable[i][j] == true <=> подслово word[i..j) лежит в языке.
     */
    struct _TState {
        std::vector< std::vector<bool> > is_reachable;
    };

    static const std::string _ERROR; /* = "ERROR" */
    static const std::string _INF;   /* = "INF"   */

    /*
     * Стек состояний _TState для подвыражений, обрабатываемых в ходе алгоритма.
     */
    std::stack<_TState> _states_stack;

    /*
     * Указатель на обрабатываемые данные.
     */
    const TData* _data;

    /*
     * Точка входа в алгоритм. Результат работы этой функции возвращает public метод solve().
     */
    std::string _solve();

    /*
     * После обработки рег. выражения на стеке остается один элемент типа _TState.
     * Этот метод находит и возвращает макс. длину префикса, который лежит в языке.
     */
    std::string _calcResult();

    /* 
     * Вспомогательный метод. Создает и возвращает матрицу _TState подходящего размера, заполненную false.
     */
    _TState _createEmptyState() const;

    /*
     * Обработать + на новой итерации алгоритма.
     */
    bool _pushPlus();
  
    /*
     * Обработать . на новой итерации алгоритма. 
     */
    bool _pushDot();
  
    /*
     * Обработать * на новой итерации алгоритма. 
     */
    bool _pushStar();
  
    /*
     * Обработать символ 'a', 'b', 'c' или '1' на новой итерации алгоритма. 
     */
    void _pushSymbol(char symbol);
};
