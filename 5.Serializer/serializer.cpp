#include "serializer.h"

Serializer::Serializer(std::ostream& out) : out_(out) {}

Error Serializer::process(const bool& arg) { // boll&& �� �������, �� ��� �������� && � �������� ������������ � lvalue
        if (arg)
            out_ << "true";
        else
            out_ << "false";
        out_  << Separator;

        if (out_.fail()) // �������� ������� ������ ������
            return Error::CorruptedArchive;

        return Error::NoError;
    }

Error Serializer::process(const uint64_t& value) {
        out_ << std::to_string(value) << Separator;

        if (out_.fail()) // �������� ������� ������ ������
            return Error::CorruptedArchive;

        return Error::NoError;
    }

    // ������� � ������ ������� ���������� ��� ��������
Error Serializer::process() {
        return Error::NoError;
    }

    // ����������� ��������� ���������� �����
    template <class T, class... Args>
    Error Serializer::process(const T& value, const Args&... args) {
        Error err = process(value); // ������ �������

        // ������ - �������
        if (err != Error::NoError) {
            return err;
        }

        // ��������� ���������
        return process(args...);
    }


    Deserializer::Deserializer(std::istream& in) : in_(in) {}

    Error Deserializer::process(bool& value)
    {
        std::string text;
        in_ >> text;

        if (in_.fail() || text.empty()) { // �������� ������� ������ ������
            return Error::CorruptedArchive;
        }

        if (text == "true")
            value = true;
        else if (text == "false")
            value = false;
        else
            return Error::CorruptedArchive;

        return Error::NoError;
    }

    Error Deserializer::process(uint64_t& value)
    {
        std::string text;
        in_ >> text;

        if (in_.fail()) { // �������� ������� ������ ������, empty � stoull ����
            return Error::CorruptedArchive;
        }

        try {
            value = std::stoull(text);
        }
        catch (...) {
            return Error::CorruptedArchive;
        }

        return Error::NoError;
    }


    // ������� � ������ ������� ���������� ��� ��������
    Error Deserializer::process() {
        return Error::NoError;
    }

    // ����������� ��������� ���������� �����
    template <class T, class... Args>
    Error Deserializer::process(T& first, Args&... args) {
        Error err = process(first); // ������ �������

        // ������ - �������
        if (err != Error::NoError) {
            return err;
        }

        // ��������� ���������
        return process(args...);
    }