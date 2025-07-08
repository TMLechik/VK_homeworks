#pragma once
#include <iostream>
#include <string>
#include <sstream> // stringstream
#include <cstdint> // Для uint64_t

enum class Error
{
    NoError,
    CorruptedArchive
};

struct Data
{
    uint64_t a;
    bool b;
    uint64_t c;

    template <class Serializer>
    Error serialize(Serializer& serializer)
    {
        return serializer(a, b, c);
    }
};

class Serializer
{
    static constexpr char Separator = ' ';
    std::ostream& out_;

    Error process(const bool& arg);
    Error process(const uint64_t& value);
    Error process();

    template <class T, class... Args>
    Error process(const T& value, const Args&... args);

public:
    explicit Serializer(std::ostream& out);

    template <class T>
    Error save(T& object)
    {
        return object.serialize(*this);
    }

    template <class... ArgsT>
    Error operator()(ArgsT... args)
    {
        return process(args...);
    }
};

class Deserializer
{
    std::istream& in_;

    Error process(bool& value);
    Error process(uint64_t& value);
    Error process();

    template <class T, class... Args>
    Error process(T& first, Args&... args);

public:
    explicit Deserializer(std::istream& in);

    template <class T>
    Error load(T& object) // если передан Deserializer& obj - то будет y.serialize<Deserializer>(obj) => operrator для Deserializer
    {
        return object.serialize(*this);
    }

    template <class... ArgsT>
    Error operator()(ArgsT&... args)
    {
        return process(args...);
    }
};
