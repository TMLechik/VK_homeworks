#include "format.h"
#include <cassert>

// ����� � ���������� operator<< ��� ����� 2
struct Point {
    int x, y;
    friend std::ostream& operator<<(std::ostream& os, const Point& p) {
        return os << "(" << p.x << "," << p.y << ")";
    }
};

void run_tests() {
    // ���� 1: �� �������
    {
        auto text = format("{1}+{1} = {0}", 2, "one");
        assert(text == "one+one = 2");
    }

    // ���� 2: ������ ���� ����������
    {
        Point p{ 10, 20 };
        auto text = format("Struct: {0}, Float: {1}, Int: {2}, String: {3}", p, 3.14,2,"Pop");
        assert(text == "Struct: (10,20), Float: 3.14, Int: 2, String: Pop");
    }

    // ���� 3: ������� ����������
    {
        auto text = format("{2}, {1}, {0}", "a", "b", "c");
        assert(text == "c, b, a");
    }

    // ���� 4: ������������ ������������� ���������
    {
        auto text = format("{0}-{1}-{0}", "lol", "kek");
        assert(text == "lol-kek-lol");
    }

    // ���� 5: ������ ������
    {
        auto text = format("", 1, 2, 3);
        assert(text == "");
    }

    // ���� 6: ������ ��� �����������
    {
        auto text = format("Hello World!", 1);
        assert(text == "Hello World!");
    }

    // ���� 7: {{0}}
    {
        try {
            auto text = format("{{0}}", 42);
        }
        catch (const std::runtime_error& e) {
            assert(std::string(e.what()) == "Invalid character in braces");
        }
    }

    // ���� 8: ���������� ������
    {
        try {
            auto text = format("{0", 1);
        }
        catch (const std::runtime_error& e) {
            assert(std::string(e.what()) == "Unclosed brace");
        }
    }

    // ���� 9: ������ ������
    {
        try {
            auto text = format("{}", 1);
        }
        catch (const std::runtime_error& e) {
            assert(std::string(e.what()) == "Empty braces");
        }
    }

    // ���� 10: ���������� ������
    {
        try {
            auto text = format("{a}", 1);
        }
        catch (const std::runtime_error& e) {
            assert(std::string(e.what()) == "Invalid character in braces");
        }
    }

    // ���� 11: ������ � ��������
    {
        try {
            auto text = format("{ 0 }", 1);
        }
        catch (const std::runtime_error& e) {
            assert(std::string(e.what()) == "Invalid character in braces");
        }
    }

    // ���� 12: ��������� ����������� ������
    {
        try {
            auto text = format("0}", 1);
        }
        catch (const std::runtime_error& e) {
            assert(std::string(e.what()) == "Unmatched '}'");
        }
    }

    // ���� 13: ������ ��������� ����� ����������
    {
        try {
            auto text = format("{10}", 1, 2, 3);
        }
        catch (const std::out_of_range& e) {
            assert(std::string(e.what()) == "Argument index out of range");
        }
    }

    // ���� 14: ������������� ������
    {
        try {
            auto text = format("{-1}", 1);
        }
        catch (const std::runtime_error& e) {
            assert(std::string(e.what()) == "Invalid character in braces");
        }
    }

    // ���� 15: ������� ���������� ����������
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
