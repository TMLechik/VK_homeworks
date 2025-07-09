#include "format.h"
#include <cassert>

// Класс с поддержкой operator<< для Теста 2
struct Point {
    int x, y;
    friend std::ostream& operator<<(std::ostream& os, const Point& p) {
        return os << "(" << p.x << "," << p.y << ")";
    }
};

void run_tests() {
    // Тест 1: Из примера
    {
        auto text = format("{1}+{1} = {0}", 2, "one");
        assert(text == "one+one = 2");
    }

    // Тест 2: Разные типы аргументов
    {
        Point p{ 10, 20 };
        auto text = format("Struct: {0}, Float: {1}, Int: {2}, String: {3}", p, 3.14,2,"Pop");
        assert(text == "Struct: (10,20), Float: 3.14, Int: 2, String: Pop");
    }

    // Тест 3: Порядок аргументов
    {
        auto text = format("{2}, {1}, {0}", "a", "b", "c");
        assert(text == "c, b, a");
    }

    // Тест 4: Многократное использование аргумента
    {
        auto text = format("{0}-{1}-{0}", "lol", "kek");
        assert(text == "lol-kek-lol");
    }

    // Тест 5: Пустая строка
    {
        auto text = format("", 1, 2, 3);
        assert(text == "");
    }

    // Тест 6: Строка без подстановки
    {
        auto text = format("Hello World!", 1);
        assert(text == "Hello World!");
    }

    // Тест 7: {{0}}
    {
        try {
            auto text = format("{{0}}", 42);
        }
        catch (const std::runtime_error& e) {
            assert(std::string(e.what()) == "Invalid character in braces");
        }
    }

    // Тест 8: Незакрытая скобка
    {
        try {
            auto text = format("{0", 1);
        }
        catch (const std::runtime_error& e) {
            assert(std::string(e.what()) == "Unclosed brace");
        }
    }

    // Тест 9: Пустые скобки
    {
        try {
            auto text = format("{}", 1);
        }
        catch (const std::runtime_error& e) {
            assert(std::string(e.what()) == "Empty braces");
        }
    }

    // Тест 10: Нечисловой индекс
    {
        try {
            auto text = format("{a}", 1);
        }
        catch (const std::runtime_error& e) {
            assert(std::string(e.what()) == "Invalid character in braces");
        }
    }

    // Тест 11: Индекс с пробелом
    {
        try {
            auto text = format("{ 0 }", 1);
        }
        catch (const std::runtime_error& e) {
            assert(std::string(e.what()) == "Invalid character in braces");
        }
    }

    // Тест 12: Одиночная закрывающая скобка
    {
        try {
            auto text = format("0}", 1);
        }
        catch (const std::runtime_error& e) {
            assert(std::string(e.what()) == "Unmatched '}'");
        }
    }

    // Тест 13: Индекс превышает число аргументов
    {
        try {
            auto text = format("{10}", 1, 2, 3);
        }
        catch (const std::out_of_range& e) {
            assert(std::string(e.what()) == "Argument index out of range");
        }
    }

    // Тест 14: Отрицательный индекс
    {
        try {
            auto text = format("{-1}", 1);
        }
        catch (const std::runtime_error& e) {
            assert(std::string(e.what()) == "Invalid character in braces");
        }
    }

    // Тест 15: Большое количество аргументов
    {
        auto text = format("{9}{8}{7}{6}{5}{4}{3}{2}{1}{0}", 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j');
        assert(text == "jihgfedcba");
    }

    std::cout << "All tests passed!\n";
}

int main() {
    run_tests();
    return 0;
}
