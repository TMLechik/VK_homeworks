#include "serializer.h"
#include <cassert>
#include <limits>

void run_tests() {
    // ���� 1: ������� ������������
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

    // ���� 2: ������������ ������ (������������ �����)
    {
        std::stringstream corrupt_stream("abc true 100");
        Data z;
        Deserializer deserializer(corrupt_stream);
        assert(deserializer.load(z) == Error::CorruptedArchive);
    }

    // ���� 3: ������������ ������ (������������ bool)
    {
        std::stringstream corrupt_stream("123 maybe 456");
        Data z;
        Deserializer deserializer(corrupt_stream);
        assert(deserializer.load(z) == Error::CorruptedArchive);
    }

    // ���� 4: ������ ������
    {
        std::stringstream empty_stream;
        Data z;
        Deserializer deserializer(empty_stream);
        assert(deserializer.load(z) == Error::CorruptedArchive);
    }

    // ���� 5: �������� ������
    {
        std::stringstream incomplete_stream("123 true");
        Data z;
        Deserializer deserializer(incomplete_stream);
        assert(deserializer.load(z) == Error::CorruptedArchive);
    }

    // ���� 6: ������ ������ (������ ��������������)
    {
        Data x{ 42, false, 100 };
        std::stringstream stream;

        Serializer serializer(stream);
        assert(serializer.save(x) == Error::NoError);
        stream << "extra data";  // ������ ������

        Data y{ 0, true, 0 };
        Deserializer deserializer(stream);
        assert(deserializer.load(y) == Error::NoError);

        assert(x.a == y.a);
        assert(x.b == y.b);
        assert(x.c == y.c);
    }

    // ���� 7: ��������� �������� �����
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
    // ���� 8: ������� ��������
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