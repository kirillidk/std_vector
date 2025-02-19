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
    vector<T>& operator=(const vector<T>& __other);

    void reserve(std::size_t __capacity);
    void push_back(const T& __value);

    std::size_t size() const noexcept;
    std::size_t capacity() const noexcept;

    T& operator[](std::size_t __index);
    const T& operator[](std::size_t __index) const;
private:
    void _M_swap(vector<T>& __other);

    T* _M_arr;
    std::size_t _M_size;
    std::size_t _M_capacity;
};

/**
 * @brief Default constructor
 */
template <typename T>
vector<T>::vector() : _M_arr(nullptr), _M_size(0), _M_capacity(0) {}

/**
 * @brief Сopy constructor
 * @param __other A const reference to another vector to construct from
 */
template <typename T>
vector<T>::vector(const vector<T>& __other) {
    T* __new_arr =
        reinterpret_cast<T*>(new char[sizeof(T) * __other._M_capacity]);

    for (std::size_t i = 0; i < __other._M_size; ++i) {
        new (__new_arr + i) T(__other._M_arr[i]);
    }

    _M_arr = __new_arr;
    _M_size = __other._M_size;
    _M_capacity = __other._M_capacity;
}

/**
 * @brief Destructor
 */
template <typename T>
vector<T>::~vector() {
    for (std::size_t i = 0; i < _M_size; ++i) {
        (_M_arr + i)->~T();
    }

    delete[] reinterpret_cast<char*>(_M_arr);
}

/**
 * @brief Replaces the contents of the container
 * @param __other Another container to use as data source
 * @return A reference to current container
 */
template <typename T>
vector<T>& vector<T>::operator=(const vector<T>& __other) {
    if (this == &__other) {
        return *this;
    }
    vector<T> tmp(__other);
    _M_swap(tmp);
    return *this;
}

/**
 * @brief Reallocates the vector with new capacity
 * @param __capacity A new capacity of the vector, in number of elements
 * @note Reallocation happens only if the new capacity exceeds the
 * current capacity
 */
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

/**
 * @brief Appends the given element value to the end of the container
 * @param __value A const reference to an element
 * @note If the current size reaches the capacity of the vector,
 *       the vector will reallocate, doubling its capacity
 */
template <typename T>
void vector<T>::push_back(const T& __value) {
    if (_M_size == _M_capacity) {
        reserve(_M_capacity ? 2 * _M_capacity : 1);
    }
    new (_M_arr + _M_size) T(__value);
    ++_M_size;
}

/**
 * @brief Returns the number of elements in the container
 */
template <typename T>
std::size_t vector<T>::size() const noexcept {
    return _M_size;
}

/**
 * @brief Returns the number of elements that the container has currently
 * allocated space for
 */
template <typename T>
std::size_t vector<T>::capacity() const noexcept {
    return _M_capacity;
}

/**
 * @brief Returns a reference to the element at specified location index
 * @param __index An index of the element to return
 * @return A reference to the requested element
 * @note No bounds checking are performed
 */
template <typename T>
T& vector<T>::operator[](std::size_t __index) {
    return _M_arr[__index];
}

/**
 * @brief Returns a const reference to the element at specified location index
 * @param __index An index of the element to return
 * @return A const reference to the requested element
 * @note No bounds checking are performed
 */
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