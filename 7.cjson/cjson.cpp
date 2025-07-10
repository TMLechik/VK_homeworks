#include <Python.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

// Пропуск ведущих пробелов
static void skip_whitespace(const char **str) {
    while (isspace((unsigned char)**str)) {
        (*str)++;
    }
}

// Парсинг string
static PyObject* parse_string(const char **str) {

    // Проверка открывающей кавычки
    if (**str != '"') {
        PyErr_SetString(PyExc_ValueError, "Expected '\"'");
        return NULL;
    }
    (*str)++; // Пропускаем открывающую кавычку
    
    const char *start = *str; // запоминаем начало строки
    while (**str && **str != '"') {
        (*str)++;
    }
    
    // Проверка закрывающей кавычки
    if (**str != '"') {
        PyErr_SetString(PyExc_ValueError, "ERROR: Unmatched string");
        return NULL;
    }
    
    PyObject *py_str = PyUnicode_FromStringAndSize(start, *str - start);
    (*str)++; // Пропускаем закрывающую кавычку
    return py_str;
}

// Парсим число
static PyObject* parse_number(const char **str) {
    const char *start = *str;
    char *end;

    // Преобразование в double
    double dval = strtod(start, &end); // end будет указывать на первый символ после числа
    if (end != start) {
        *str = end;
        return PyFloat_FromDouble(dval);
    }
    
    PyErr_SetString(PyExc_ValueError, "ERROR: Invalid number");
    return NULL;
}

// Парсим значение (строка, число)
static PyObject* parse_value(const char **str) {

     skip_whitespace(str);
    
    if (**str == '"') {
        return parse_string(str);
    } else if (isdigit(**str)) {
        return parse_number(str);
    } else if (**str == '-' && isdigit((*str)[1])) {
        return parse_number(str);
    }
    PyErr_SetString(PyExc_ValueError, "ERROR: Unexpected token");
    return NULL;
}

// Метод loads: JSON -> словарь Python
static PyObject* loads(PyObject *self, PyObject *args) {
    const char *json_str;
    if (!PyArg_ParseTuple(args, "s", &json_str)) {
        return NULL;
    }
    const char *ptr = json_str;
    skip_whitespace(&ptr);
    
    if (*ptr != '{') {
        PyErr_SetString(PyExc_ValueError, "ERROR: Expected '{'");
        return NULL;
    }
    ptr++;
    
    PyObject *dict = PyDict_New();
    if (!dict) {
        PyErr_SetString(PyExc_ValueError, "ERROR: Failed to create Dict Object");
        return NULL;
    }
    
    skip_whitespace(&ptr);
    while (*ptr != '}') {
        // Парсим ключ
        PyObject *key = parse_string(&ptr);
        if (!key) {
            PyErr_SetString(PyExc_ValueError, "ERROR: Failed to build string value");
            Py_DECREF(dict);
            return NULL;
        }
        
        skip_whitespace(&ptr);
        if (*ptr != ':') {
            PyErr_SetString(PyExc_ValueError, "ERROR: Expected ':'");
            Py_DECREF(key);
            Py_DECREF(dict);
            return NULL;
        }
        ptr++;
        
        // Парсим значение
        PyObject *value = parse_value(&ptr);
        if (!value) {
            PyErr_SetString(PyExc_ValueError, "ERROR: Failed to build value");
            Py_DECREF(key);
            Py_DECREF(dict);
            return NULL;
        }
        
        // Добавляем пару в словарь
        if (PyDict_SetItem(dict, key, value) < 0) {
            PyErr_SetString(PyExc_ValueError, "ERROR: Failed to set item");
            Py_DECREF(key);
            Py_DECREF(value);
            Py_DECREF(dict);
            return NULL;
        }
        
        Py_DECREF(key);
        Py_DECREF(value);
        
        skip_whitespace(&ptr);
        if (*ptr == ',') {
            ptr++;
            skip_whitespace(&ptr);
        } else if (*ptr != '}') {
            PyErr_SetString(PyExc_ValueError, "ERROR: Expected ',' or '}'");
            Py_DECREF(dict);
            return NULL;
        }
    }
    ptr++; // Пропускаем закрывающую '}'
    
    return dict;
}

// Метод dumps: словарь Python -> JSON
static PyObject* dumps(PyObject *self, PyObject *args) {
    PyObject *dict;
    if (!PyArg_ParseTuple(args, "O", &dict)) { // О - любой объект python
        return NULL;
    }
    
    if (!PyDict_Check(dict)) {
        PyErr_SetString(PyExc_TypeError, "ERROR: Expected a dictionary");
        return NULL;
    }
    
    PyObject *json_str = PyUnicode_FromString("{");
    if (!json_str) {
        return NULL;
    }
    
    Py_ssize_t pos = 0;
    PyObject *key, *value;
    int first = 1;
    
    while (PyDict_Next(dict, &pos, &key, &value)) {
        // Проверяем тип ключа
        if (!PyUnicode_Check(key)) {
            PyErr_SetString(PyExc_TypeError, "ERROR: Key must be string");
            Py_DECREF(json_str);
            return NULL;
        }
        
        // Добавляем запятую перед элементами, кроме первого
        if (!first) {
            PyObject *comma = PyUnicode_FromString(",");
            PyUnicode_Append(&json_str, comma);
            Py_DECREF(comma);
            if (!json_str) {
                PyErr_SetString(PyExc_TypeError, "ERROR: PyUnicode_Append fail");
                return NULL; // PyUnicode_Append в случае ошибки устанавливает указатель json_str в NULL
            }
        }
        first = 0;
        
        // Сериализуем ключ
        PyObject *quoted_key = PyUnicode_FromFormat("\"%U\"", key);
        PyUnicode_Append(&json_str, quoted_key);
        Py_DECREF(quoted_key);
        if (!json_str) {
                PyErr_SetString(PyExc_TypeError, "ERROR: PyUnicode_Append fail");
                return NULL;
            }
        
        // Добавляем двоеточие
        PyObject *colon = PyUnicode_FromString(":");
        PyUnicode_Append(&json_str, colon);
        Py_DECREF(colon);
        if (!json_str) {
                PyErr_SetString(PyExc_TypeError, "ERROR: PyUnicode_Append fail");
                return NULL;
            }
        
        // Сериализуем значение
        PyObject *value_str = NULL;
        if (PyUnicode_Check(value)) {
            value_str = PyUnicode_FromFormat("\"%U\"", value);
        } else if (PyFloat_Check(value)) {
            value_str = PyObject_Str(value);
        } else if (PyLong_Check(value)) {
            value_str = PyObject_Str(value);
        }else {
            PyErr_SetString(PyExc_TypeError, "ERROR: Unsupported value type");
            Py_DECREF(json_str);
            return NULL;
        }
        
        
        PyUnicode_Append(&json_str, value_str);
        Py_DECREF(value_str);
        if (!json_str) {
                PyErr_SetString(PyExc_TypeError, "ERROR: PyUnicode_Append fail");
                return NULL;
            }
    }
    
    PyObject *closing = PyUnicode_FromString("}");
    PyUnicode_Append(&json_str, closing);
    Py_DECREF(closing);
    
    return json_str;
}

// Определение методов модуля
static PyMethodDef cjson_methods[] = {
    {"loads", loads, METH_VARARGS, "JSON string into a Python dict"},
    {"dumps", dumps, METH_VARARGS, "Python dict to JSON string"},
    {NULL, NULL, 0, NULL}
};

// Определение модуля
static struct PyModuleDef cjson_module = {
    PyModuleDef_HEAD_INIT,
    "cjson",
    "JSON parser and serializer",
    -1,
    cjson_methods
};

// Инициализация модуля
PyMODINIT_FUNC PyInit_cjson(void) {
    return PyModule_Create(&cjson_module);
}