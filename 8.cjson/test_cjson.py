import cjson
import json
import ujson
import time
import random
import string

def run_tests():
    # Тест 1: Простой парсинг
    json_str = '{"name": "John", "age": 30}'
    parsed = cjson.loads(json_str)
    assert (parsed == {'name': 'John', 'age': 30.0})

    # Тест 2: Простая сериализация
    data = {
        "name": "Alice",
        "age": 25
    }
    serialized = cjson.dumps(data)
    assert (serialized == '{"name":"Alice","age":25}')

    # Тест 3: Комбинация разных типов значений - сериализация и парсинг
    data = {
        "string": "text",
        "int": 42,
        "float": 3.14,
        "negative": -10,
        "large_number": 10000000000
    }
    serialized = cjson.dumps(data)
    parsed = cjson.loads(serialized)
    assert parsed['string'] == 'text'
    assert parsed['int'] == 42.0
    assert parsed['float'] == 3.14
    assert parsed['negative'] == -10.0
    assert parsed['large_number'] == 10000000000.0


    # Тест 4: Пустой ключ и пустое значение
    json_str = '{"": "empty key", "empty_value": ""}'
    parsed = cjson.loads(json_str)
    assert parsed[''] == 'empty key'
    assert parsed['empty_value'] == ''

    # Тест 5: Ошибка парсинга- неверный формат строки
    try:
        cjson.loads('{invalid json}')
    except Exception as e:
        assert (str(e) == 'ERROR: Failed to build string value')

     # Тест 6: Ошибка сериализации - неподдерживаемый тип
    try:
        cjson.dumps({"key": [1, 2, 3]})
    except Exception as e:
        assert (str(e) == 'ERROR: Unsupported value type')

    # Тест 7: Ошибка - лишняя запятая
    try:
        cjson.loads('{"a": 1, "b": 2,}')
    except Exception as e:
        assert (str(e) == 'ERROR: Unexpected token')

    print("All tests passed!")


#### Тест производительности

# Генератор случайных строк
def random_string(length=10):
    return ''.join(random.choices(string.ascii_letters + string.digits, k=length))

test_data = {}

# string:string
for i in range(50000):
    key = f"str_key_{i}"
    test_data[key] = random_string()

# string:int
for i in range(50000):
    key = f"int_key_{i}"
    test_data[key] = random.randint(0, 100000)


# Замер времени
def measure_operation(name, func, *args): #  Измерение времени операции
    start_time = time.perf_counter_ns()
    result = func(*args)
    end_time = time.perf_counter_ns()
    duration = end_time - start_time
    print(f"{name}: {duration/1000000:.3f} ms")
    return result

def run_time_tests():

    print("\n===DUMPS===")
    cjson_result = measure_operation("cjson.dumps", cjson.dumps, test_data)
    json_result = measure_operation("json.dumps", json.dumps, test_data)
    ujson_result = measure_operation("ujson.dumps", ujson.dumps, test_data)
    
    json_str = cjson_result;

    print("\n===LOADS===")
    measure_operation("cjson.loads", cjson.loads, json_str)
    measure_operation("json.loads", json.loads, json_str)
    measure_operation("ujson.loads", ujson.loads, json_str)
    

if __name__ == "__main__":
    run_tests()
    run_time_tests()
