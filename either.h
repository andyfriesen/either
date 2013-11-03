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

    Either(const Left& l)
        : state(State::L)
    {
        new (&storage) Left(l);
    }

    Either(const Right& r)
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
            default:
                abort();
        }
    }

    Either(Either&& r)
        : state(r.state)
    {
        switch (state) {
            case State::L:
                new (&storage) Left(std::move(r.left()));
                break;
            case State::R:
                new (&storage) Right(std::move(r.right()));
                break;
        }
    }

    Either(Left&& l)
        : state(State::L)
    {
        new (&storage) Left(std::move(l));
    }

    Either(Right&& r)
        : state(State::R)
    {
        new (&storage) Right(std::move(r));
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

    Either& operator = (const Either& rhs) {
        if (&rhs != this) {
            this->~Either();
            new (this) Either(rhs);
        }
        return *this;
    }

    Either& operator = (Either&& rhs) {
        if (&rhs != this) {
            this->~Either();
            new (this) Either(std::move(rhs));
        }
        return *this;
    }

    template <typename R, typename LF, typename RF>
    R match(
        LF leftFunc,
        RF rightFunc
    ) {
        switch (state) {
            case State::L:
                return leftFunc(left());
            case State::R:
                return rightFunc(right());
            default:
                abort();
        }
    }

    template <typename R, typename LF, typename RF>
    R match(
        LF leftFunc,
        RF rightFunc
    ) const {
        switch (state) {
            case State::L:
                return leftFunc(left());
            case State::R:
                return rightFunc(right());
            default:
                abort();
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
            default:
                abort();
        }
    }

    bool operator != (const Either& rhs) const {
        return !(*this == rhs);
    }

    bool isLeft() const {
        return State::L == state;
    }

private:
    const Left& left()   const { return *reinterpret_cast<const Left*>(&storage); }
    const Right& right() const { return *reinterpret_cast<const Right*>(&storage); }

    Left& left()   { return *reinterpret_cast<Left*>(&storage); }
    Right& right() { return *reinterpret_cast<Right*>(&storage); }

    enum class State {
        L, R
    };

    State state;

    typename std::aligned_storage<detail::MaxSize<Left, Right>::result>::type storage;
};

template <typename L, typename R>
bool isLeft(const Either<L, R>& e) {
    return e.isLeft();
}

template <typename L, typename R>
bool isRight(const Either<L, R>& e) {
    return !isLeft(e);
}

template <typename L, typename R>
L& left(Either<L, R>& e) {
    return e.template match<L&>(
        [](L& l) -> L& { return l; },
        [](R& r) -> L& { printf("Unexpected\n"); abort(); return *(L*)0; });
}

template <typename L, typename R>
const L& left(const Either<L, R>& e) {
    return e.template match<const L&>(
        [](const L& l) -> const L& { return l; },
        [](const R& r) -> const L& { printf("Unexpected\n"); abort(); return *(L*)0; });
}

template <typename L, typename R>
R& right(Either<L, R>& e) {
    return e.template match<R&>(
        [](L& l) -> R& { printf("Unexpected\n"); abort(); return *(R*)0; },
        [](R& r) -> R& { return r; });
}


template <typename L, typename R>
const R& right(const Either<L, R>& e) {
    return e.template match<const R&>(
        [](const L& l) -> const R& { printf("Unexpected\n"); abort(); return *(R*)0; },
        [](const R& r) -> const R& { return r; });
}
