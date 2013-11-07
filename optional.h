#pragma once

#include <functional>
#include <type_traits>

template <typename T>
struct Optional {
    Optional()
        : hasvalue(false)
    {}

    explicit Optional(const T& l)
        : hasvalue(true)
    {
        new (&storage) T(l);
    }

    explicit Optional(T&& l)
        : hasvalue(true)
    {
        new (&storage) T(std::move(l));
    }

    Optional(const Optional& rhs)
        : hasvalue(rhs.hasvalue)
    {
        if (rhs.hasvalue) {
            new (&storage) T(rhs.value());
        }
    }

    Optional(Optional&& rhs)
        : hasvalue(rhs.hasvalue)
    {
        if (rhs.hasvalue) {
            new (&storage) T(rhs.value());
        }
    }

    ~Optional() {
        if (hasvalue) {
            value().~T();
        }
    }

    Optional& operator = (const Optional& rhs) {
        if (this != &rhs) {
            this->~Optional();
            new (this) Optional(rhs);
        }
    }

    Optional& operator = (Optional&& rhs) {
        if (this != &rhs) {
            this->~Optional();
            new (this) Optional(rhs);
        }
    }

    const T& value() const {
        if (!hasvalue) {
            abort();
        }
        return *reinterpret_cast<const T*>(&storage);
    }

    T& value() {
        if (!hasvalue) {
            abort();
        }
        return *reinterpret_cast<T*>(&storage);
    }

    bool hasValue() const {
        return hasvalue;
    }

private:
    bool hasvalue;

    typename std::aligned_storage<sizeof(T), alignof(T)>::type storage;
};
