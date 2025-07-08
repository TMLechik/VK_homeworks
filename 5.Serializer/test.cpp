#include "serializer.h"
#include <cassert>
#include <limits>

void run_tests() {
    // Тест 1: Базовая сериализация
    {
        Data x{ 1, true, 2 };
        std::stringstream stream;

        Serializer serializer(stream);
        assert(serializer.save(x) == Error::NoError);

        Data y{ 0, false, 0 };
        Deserializer deserializer(stream);
        assert(deserializer.load(y) == Error::NoError);

        assert(x.a == y.a);
        assert(x.b == y.b);
        assert(x.c == y.c);
    }

    // Тест 2: Поврежденные данные (некорректное число)
    {
        std::stringstream corrupt_stream("abc true 100");
        Data z;
        Deserializer deserializer(corrupt_stream);
        assert(deserializer.load(z) == Error::CorruptedArchive);
    }

    // Тест 3: Поврежденные данные (некорректный bool)
    {
        std::stringstream corrupt_stream("123 maybe 456");
        Data z;
        Deserializer deserializer(corrupt_stream);
        assert(deserializer.load(z) == Error::CorruptedArchive);
    }

    // Тест 4: Пустые данные
    {
        std::stringstream empty_stream;
        Data z;
        Deserializer deserializer(empty_stream);
        assert(deserializer.load(z) == Error::CorruptedArchive);
    }

    // Тест 5: Неполные данные
    {
        std::stringstream incomplete_stream("123 true");
        Data z;
        Deserializer deserializer(incomplete_stream);
        assert(deserializer.load(z) == Error::CorruptedArchive);
    }

    // Тест 6: Лишние данные (должны игнорироваться)
    {
        Data x{ 42, false, 100 };
        std::stringstream stream;

        Serializer serializer(stream);
        assert(serializer.save(x) == Error::NoError);
        stream << "extra data";  // Лишние данные

        Data y{ 0, true, 0 };
        Deserializer deserializer(stream);
        assert(deserializer.load(y) == Error::NoError);

        assert(x.a == y.a);
        assert(x.b == y.b);
        assert(x.c == y.c);
    }

    // Тест 7: Граничные значения чисел
    {
        const uint64_t max = std::numeric_limits<uint64_t>::max();
        const uint64_t min = 0;

        Data x{ max, true, min };
        std::stringstream stream;

        Serializer serializer(stream);
        assert(serializer.save(x) == Error::NoError);

        Data y{ 0, false, 0 };
        Deserializer deserializer(stream);
        assert(deserializer.load(y) == Error::NoError);

        assert(x.a == y.a && x.a == max);
        assert(x.b == y.b);
        assert(x.c == y.c && x.c == min);
    }
    // Тест 8: Нулевые значения
    {
        Data x{ 0, false, 0 };
        std::stringstream stream;

        Serializer serializer(stream);
        assert(serializer.save(x) == Error::NoError);

        Data y{ 1, true, 1 };
        Deserializer deserializer(stream);
        assert(deserializer.load(y) == Error::NoError);

        assert(y.a == 0);
        assert(y.b == false);
        assert(y.c == 0);
    }

    std::cout << "All tests passed!\n";
}

int main() {
    run_tests();
    return 0;
}