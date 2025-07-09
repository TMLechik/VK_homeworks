#pragma once

#include <iostream>
#include <sstream> // std::ostringstream
#include <string> // std::stoul
#include <tuple>
#include <utility> // std::forward
#include <cctype> // std::isdigit
#include <stdexcept>
#include <type_traits> // std::is_convertible_v

// ������� �� ��������� <<
template <typename T>
concept OutputStreamable = requires(std::ostream & os, T && arg) {
    { os << std::forward<T>(arg) } -> std::convertible_to<std::ostream&>;
};

// ��������������� ������� ��� ������ �� �������
template <size_t I, typename Tuple>
void print_arg(std::ostringstream& oss, const Tuple& args) {
    oss << std::get<I>(args);
}

template <size_t I = 0, typename Tuple>
void print_arg_by_index(std::ostringstream& oss, size_t target_idx, const Tuple& args) {
    if constexpr (I < std::tuple_size_v<Tuple>) { // else  �� ������������� ��� I< � ��������
        if (I == target_idx) {
            print_arg<I>(oss, args);
        }
        else {
            print_arg_by_index<I + 1>(oss, target_idx, args);
        }
    }
    else {
        throw std::out_of_range("Argument index out of range");
    }
}

// �������� ������� ��������������
template <typename... Args>
std::string format(const std::string& fmt, Args&&... args) requires(OutputStreamable<Args> && ...)
{
    const auto args_tuple = std::forward_as_tuple(std::forward<Args>(args)...); // tuple - ������ ��� ���������
    std::ostringstream result;
    size_t pos = 0; // ��������� �� fmt

    while (pos < fmt.size()) {
        if (fmt[pos] == '{') {
            size_t end = pos + 1;
            std::string index_str;

            // ���� ���� ������ ������
            while (end < fmt.size() && fmt[end] != '}') {
                if (!std::isdigit(static_cast<unsigned char>(fmt[end]))) {
                    throw std::runtime_error("Invalid character in braces");
                }
                index_str += fmt[end++];
            }

            // �������� ������
            if (end == fmt.size()) throw std::runtime_error("Unclosed brace");
            if (index_str.empty()) throw std::runtime_error("Empty braces");

            // �������������� � �����
            size_t index_int;
            try {
                index_int = std::stoul(index_str);
            }
            catch (...) {
                throw std::runtime_error("Invalid index format");
            }

            // �������� ���������
            if (index_int >= sizeof...(Args)) {
                throw std::out_of_range("Argument index out of range");
            }

            // ������� ��������
            print_arg_by_index(result, index_int, args_tuple);

            pos = end + 1;  // ��������� ����� '}'
        }
        else if (fmt[pos] == '}') {
            throw std::runtime_error("Unmatched '}'");
        }
        else {
            result << fmt[pos++];
        }
    }
    return result.str();
}
