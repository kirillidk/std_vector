#pragma once

#include <algorithm>  // std::swap
#include <cstddef>    // std::size_t

namespace kirillidk_containers {

template <typename T>
class vector {
public:
    vector();
    vector(const vector<T>& __other);
    ~vector();

    void reserve(std::size_t __capacity);
    void push_back(const T& __other);

    std::size_t size() const;
    std::size_t capacity() const;

    vector<T>& operator=(const vector<T>& __other);
    T& operator[](std::size_t __index);
    const T& operator[](std::size_t __index) const;
private:
    void _M_swap(vector<T>& __other);

    T* _M_arr;
    std::size_t _M_size;
    std::size_t _M_capacity;
};

template <typename T>
vector<T>::vector() : _M_arr(nullptr), _M_size(0), _M_capacity(0) {}

template <typename T>
vector<T>::vector(const vector<T>& __other) {
    T* __new_arr =
        reinterpret_cast<T*>(new char[sizeof(T) * __other._M_capacity]);

    for (std::size_t i = 0; i < __other._M_size; ++i) {
        new (__new_arr + i) T(__other._M_arr[i]);
    }

    _M_arr = __new_arr;
    _M_capacity = __other._M_capacity;
    _M_size = __other._M_size;
}

template <typename T>
vector<T>::~vector() {
    for (std::size_t i = 0; i < _M_size; ++i) {
        (_M_arr + i)->~T();
    }

    delete[] reinterpret_cast<char*>(_M_arr);
}

template <typename T>
void vector<T>::reserve(std::size_t __capacity) {
    if (__capacity <= _M_capacity) {
        return;
    }
    T* __new_arr = reinterpret_cast<T*>(new char[sizeof(T) * __capacity]);

    for (std::size_t i = 0; i < _M_size; ++i) {
        new (__new_arr + i) T(_M_arr[i]);
    }

    for (std::size_t i = 0; i < _M_size; ++i) {
        (_M_arr + i)->~T();
    }

    delete[] reinterpret_cast<char*>(_M_arr);

    _M_arr = __new_arr;
    _M_capacity = __capacity;
}

template <typename T>
void vector<T>::push_back(const T& __other) {
    if (_M_size == _M_capacity) {
        reserve(_M_capacity ? 2 * _M_capacity : 1);
    }
    _M_arr[_M_size++] = __other;
}

template <typename T>
std::size_t vector<T>::size() const {
    return _M_size;
}

template <typename T>
std::size_t vector<T>::capacity() const {
    return _M_capacity;
}

template <typename T>
vector<T>& vector<T>::operator=(const vector<T>& __other) {
    if (this == &__other) {
        return *this;
    }
    vector<T> tmp = __other;
    _M_swap(tmp);
    return *this;
}

template <typename T>
T& vector<T>::operator[](std::size_t __index) {
    return _M_arr[__index];
}

template <typename T>
const T& vector<T>::operator[](std::size_t __index) const {
    return _M_arr[__index];
}

template <typename T>
void vector<T>::_M_swap(vector<T>& __other) {
    std::swap(_M_arr, __other._M_arr);
    std::swap(_M_size, __other._M_size);
    std::swap(_M_capacity, __other._M_capacity);
}

}