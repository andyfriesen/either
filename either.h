#pragma once

#include <functional>
#include <type_traits>

namespace detail {
    template <typename ...TList> struct MaxSize;

    template <typename T>
    struct MaxSize<T> {
        enum { result = sizeof(T) };
    };

    template <typename Head, typename ...Tail>
    struct MaxSize<Head, Tail...> {
        enum { result = sizeof(Head) > MaxSize<Tail...>::result
            ? sizeof(Head)
            : MaxSize<Tail...>::result
        };
    };
}

template <typename Left, typename Right>
struct Either {
    Either() = delete;

    explicit Either(const Left& l)
        : state(State::L)
    {
        new (&storage) Left(l);
    }

    explicit Either(const Right& r)
        : state(State::R)
    {
        new (&storage) Right(r);
    }

    Either(const Either& rhs)
        : state(rhs.state)
    {
        switch (state) {
            case State::L:
                new (&storage)Left(rhs.left());
                break;
            case State::R:
                new (&storage)Right(rhs.right());
                break;
        }
    }

    ~Either() {
        switch (state) {
            case State::L:
                left().~Left();
                break;
            case State::R:
                right().~Right();
                break;
        }
    }

    void match(
        std::function<void (Left&)> leftFunc,
        std::function<void (Right&)> rightFunc
    ) {
        switch (state) {
            case State::L:
                return leftFunc(left());
            case State::R:
                return rightFunc(right());
        }
    }

    template <typename R>
    R match(
        std::function<R (Left&)> leftFunc,
        std::function<R (Right&)> rightFunc
    ) {
        switch (state) {
            case State::L:
                return leftFunc(left());
            case State::R:
                return rightFunc(right());
        }
    }

    void match(
        std::function<void (const Left&)> leftFunc,
        std::function<void (const Right&)> rightFunc
    ) const {
        switch (state) {
            case State::L:
                return leftFunc(left());
            case State::R:
                return rightFunc(right());
        }
    }

    template <typename R>
    R match(
        std::function<R (const Left&)> leftFunc,
        std::function<R (const Right&)> rightFunc
    ) const {
        switch (state) {
            case State::L:
                return leftFunc(left());
            case State::R:
                return rightFunc(right());
        }
    }

    bool operator == (const Either& rhs) const {
        if (state != rhs.state) {
            return false;
        }

        switch (state) {
            case State::L:
                return left() == rhs.left();
            case State::R:
                return right() == rhs.right();
        }
    }

    bool operator != (const Either& rhs) const {
        return !(*this == rhs);
    }

private:
    const Left& left() const   { return *reinterpret_cast<const Left*>(&storage); }
    const Right& right() const { return *reinterpret_cast<const Right*>(&storage); }

    Left& left()   { return *reinterpret_cast<Left*>(&storage); }
    Right& right() { return *reinterpret_cast<Right*>(&storage); }

    enum class State {
        L, R
    };

    State state;
    typedef typename std::aligned_storage<detail::MaxSize<Left, Right>::result>::type StorageType;

    StorageType storage;
};
