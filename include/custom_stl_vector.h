#pragma once

#include <algorithm>  // std::swap
#include <cstddef>    // std::size_t
#include <utility>    // std::move, std::move_if_noexcept

namespace kirillidk_containers {

template <typename T>
class vector {
public:
    // types
    typedef T value_type;
    typedef T& reference;
    typedef const T& const_reference;
    typedef std::size_t size_type;
public:
    // construct/copy/destroy
    vector();
    explicit vector(size_type __count);
    vector(const vector<T>& __other);
    vector(vector<T>&& __other) noexcept;
    ~vector();
    vector<T>& operator=(const vector<T>& __other);
    vector<T>& operator=(vector<T>&& __other) noexcept;

    // capacity
    bool empty() const noexcept;
    size_type size() const noexcept;
    size_type capacity() const noexcept;
    void reserve(size_type __capacity);

    // element access
    reference operator[](size_type __index);
    const_reference operator[](size_type __index) const;

    // modifiers
    void clear();
    void push_back(const T& __value);
    void push_back(T&& __value);
    void pop_back();
    void resize(size_type __count);
private:
    void _M_swap(vector<T>& __other);

    T* _M_arr;
    size_type _M_size;
    size_type _M_capacity;
};

/**
 * @brief Default constructor
 */
template <typename T>
vector<T>::vector() : _M_arr(nullptr), _M_size(0), _M_capacity(0) {}

/**
 * @brief Constructs a vector with __count default-inserted objects of T
 * @param __count The number of default-inserted objects to create in the vector
 * @note T should be DefaultInsertible into std::vector<T>, otherwise behaviour
 * is undefined
 */
template <typename T>
vector<T>::vector(size_type __count) {
    T* __new_arr = reinterpret_cast<T*>(new char[sizeof(T) * __count]);

    for (size_type __i = 0; __i < __count; ++__i) {
        new (__new_arr + __i) T();
    }

    _M_arr = __new_arr;
    _M_size = __count;
    _M_capacity = __count;
}

/**
 * @brief Сopy constructor
 * @param __other A const reference to another vector to construct from
 */
template <typename T>
vector<T>::vector(const vector<T>& __other) {
    T* __new_arr =
        reinterpret_cast<T*>(new char[sizeof(T) * __other._M_capacity]);

    for (size_type __i = 0; __i < __other._M_size; ++__i) {
        new (__new_arr + __i) T(__other._M_arr[__i]);
    }

    _M_arr = __new_arr;
    _M_size = __other._M_size;
    _M_capacity = __other._M_capacity;
}

/**
 * @brief Move constructor
 * @param __other A r-value reference to another vector to construct from
 * @note Moves __other's content into new container. __other is in a valid but
 * unspecified state afterwards
 */
template <typename T>
vector<T>::vector(vector<T>&& __other) noexcept
    : _M_arr(__other._M_arr),
      _M_size(__other._M_size),
      _M_capacity(__other._M_capacity) {
    __other._M_arr = nullptr;
    __other._M_size = 0;
    __other._M_capacity = 0;
}

/**
 * @brief Destructor
 */
template <typename T>
vector<T>::~vector() {
    for (size_type __i = 0; __i < _M_size; ++__i) {
        (_M_arr + __i)->~T();
    }

    delete[] reinterpret_cast<char*>(_M_arr);
}

/**
 * @brief Copy assignment operator
 * @param __other A const reference to another container
 * @return A reference to current container
 */
template <typename T>
vector<T>& vector<T>::operator=(const vector<T>& __other) {
    if (this == &__other) {
        return *this;
    }
    vector<T> __tmp(__other);
    _M_swap(__tmp);
    return *this;
}

/**
 * @brief Move assignment operator
 * @param __other A r-value reference to another vector to construct from
 * @return A reference to current container
 * @note Moves __other's content into this container. __other is in a valid but
 * unspecified state afterwards
 */
template <typename T>
vector<T>& vector<T>::operator=(vector<T>&& __other) noexcept {
    if (this == &__other) {
        return *this;
    }

    for (size_type __i = 0; __i < _M_size; ++__i) {
        (_M_arr + __i)->~T();
    }
    delete[] reinterpret_cast<char*>(_M_arr);

    _M_arr = __other._M_arr;
    _M_size = __other._M_size;
    _M_capacity = __other._M_capacity;

    __other._M_arr = nullptr;
    __other._M_size = 0;
    __other._M_capacity = 0;

    return *this;
}

/**
 * @brief Reallocates the vector with new capacity
 * @param __capacity A new capacity of the vector, in number of elements
 * @note Reallocation happens only if the new capacity exceeds the
 * current capacity
 */
template <typename T>
void vector<T>::reserve(size_type __capacity) {
    if (__capacity <= _M_capacity) {
        return;
    }
    T* __new_arr = reinterpret_cast<T*>(new char[sizeof(T) * __capacity]);

    size_type __i = 0;
    try {
        for (; __i < _M_size; ++__i) {
            new (__new_arr + __i) T(std::move_if_noexcept(_M_arr[__i]));
        }
    } catch (...) {
        for (size_type __new_i = 0; __new_i < __i; ++__new_i) {
            (__new_arr + __i)->~T();
        }
        delete[] reinterpret_cast<char*>(__new_arr);

        throw;
    }

    for (size_type __i = 0; __i < _M_size; ++__i) {
        (_M_arr + __i)->~T();
    }

    delete[] reinterpret_cast<char*>(_M_arr);

    _M_arr = __new_arr;
    _M_capacity = __capacity;
}

/**
 * @brief Erases all elements from the container
 */
template <typename T>
void vector<T>::clear() {
    for (size_type __i = 0; __i < _M_size; ++__i) {
        (_M_arr + __i)->~T();
    }
    _M_size = 0;
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
        size_type __new_cap = _M_capacity ? 2 * _M_capacity : 1;
        T* __new_arr = reinterpret_cast<T*>(new char[sizeof(T) * __new_cap]);

        size_type __i = 0;
        try {
            new (__new_arr + _M_size) T(__value);
            for (; __i < _M_size; ++__i) {
                new (__new_arr + __i) T(std::move_if_noexcept(_M_arr[__i]));
            }
        } catch (...) {
            size_type __new_i = 0;
            for (; __new_i < __i; ++__new_i) {
                (__new_arr + __new_i)->~T();
            }

            delete[] reinterpret_cast<char*>(__new_arr);

            throw;
        }

        for (size_type __i = 0; __i < _M_size; ++__i) {
            (_M_arr + __i)->~T();
        }

        delete[] reinterpret_cast<char*>(_M_arr);

        _M_arr = __new_arr;
        _M_capacity = __new_cap;
    } else {
        new (_M_arr + _M_size) T(__value);
    }
    ++_M_size;
}

template <typename T>
void vector<T>::push_back(T&& __value) {
    if (_M_size == _M_capacity) {
        size_type __new_cap = _M_capacity ? 2 * _M_capacity : 1;
        T* __new_arr = reinterpret_cast<T*>(new char[sizeof(T) * __new_cap]);

        size_type __i = 0;
        try {
            new (__new_arr + _M_size) T(std::move(__value));
            for (; __i < _M_size; ++__i) {
                new (__new_arr + __i) T(std::move_if_noexcept(_M_arr[__i]));
            }
        } catch (...) {
            size_type __new_i = 0;
            for (; __new_i < __i; ++__new_i) {
                (__new_arr + __new_i)->~T();
            }

            delete[] reinterpret_cast<char*>(__new_arr);

            throw;
        }

        for (size_type __i = 0; __i < _M_size; ++__i) {
            (_M_arr + __i)->~T();
        }

        delete[] reinterpret_cast<char*>(_M_arr);

        _M_arr = __new_arr;
        _M_capacity = __new_cap;
    } else {
        new (_M_arr + _M_size) T(std::move(__value));
    }
    ++_M_size;
}

/**
 * @brief Removes the last element of the container
 * @note Calling pop_back on an empty container results in undefined behavior
 */
template <typename T>
void vector<T>::pop_back() {
    (_M_arr + (--_M_size))->~T();
}

/**
 * @brief Resizes the container to contain __count elements
 * @param __count A new size of the container
 * @note If the current size is greater than __count, the container is reduced
 * to its first __count elements. If the current size is less than __count, then
 * additional default-inserted elements are appended
 */
template <typename T>
void vector<T>::resize(size_type __count) {
    if (__count == _M_size) return;
    if (__count < _M_size) {
        for (size_type __i = __count; __i < _M_size; ++__i) {
            (_M_arr + __i)->~T();
        }
    } else {
        if (__count > _M_capacity) {
            size_type __new_capacity = _M_capacity;
            while (__count > __new_capacity) __new_capacity *= 2;
            reserve(__new_capacity);
        }
        for (size_type __i = _M_size; __i < __count; ++__i) {
            new (_M_arr + __i) T();
        }
    }
    _M_size = __count;
}

/**
 * @brief Checks if the container has no elements
 */
template <typename T>
bool vector<T>::empty() const noexcept {
    return (_M_size == 0);
}

/**
 * @brief Returns the number of elements in the container
 */
template <typename T>
typename vector<T>::size_type vector<T>::size() const noexcept {
    return _M_size;
}

/**
 * @brief Returns the number of elements that the container has currently
 * allocated space for
 */
template <typename T>
typename vector<T>::size_type vector<T>::capacity() const noexcept {
    return _M_capacity;
}

/**
 * @brief Returns a reference to the element at specified location index
 * @param __index An index of the element to return
 * @return A reference to the requested element
 * @note No bounds checking are performed
 */
template <typename T>
typename vector<T>::reference vector<T>::operator[](size_type __index) {
    return _M_arr[__index];
}

/**
 * @brief Returns a const reference to the element at specified location index
 * @param __index An index of the element to return
 * @return A const reference to the requested element
 * @note No bounds checking are performed
 */
template <typename T>
typename vector<T>::const_reference vector<T>::operator[](size_type __index
) const {
    return _M_arr[__index];
}

template <typename T>
void vector<T>::_M_swap(vector<T>& __other) {
    std::swap(_M_arr, __other._M_arr);
    std::swap(_M_size, __other._M_size);
    std::swap(_M_capacity, __other._M_capacity);
}
}