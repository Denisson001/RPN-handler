#pragma once

#include <string>

/* 
 * Структура инпута.
 * */
struct TData {
	/*
	 * Регулярное выражение.
	 * */
    std::string expr;
	/*
	 * Слово, максимальный префикс которого ищем в языке, задаваемом рег. выражением.
	 * */
    std::string word;
};
