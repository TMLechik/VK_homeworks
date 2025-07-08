#include "serializer.h"
#include <cassert>

void run_tests() {
    // Тест 1: Базовая сериализация
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

    // Тест 2: Поврежденные данные
    std::stringstream corrupt_stream("abc true 100");
    Data z;
    assert(deserializer.load(z) == Error::CorruptedArchive);

    std::cout << "All tests passed!\n";
}

int main() {
    run_tests();
    return 0;
}