#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory>
#include <iostream>
#include "either.h"
#include "TestFramework.h"

void test_intfloat() {
    auto ef = Either<int, float>::left(5);
    auto es = Either<int, float>::right(5.0f);

    CHECK(isLeft(ef));
    CHECK(isRight(es));

    auto double_ = [](const Either<int, float>& ef) -> double {
        return ef.match<double>(
            [](int i)   { return i * 2; },
            [](float f) { return f * 2; }
        );
    };

    double r = double_(ef);
    double s = double_(es);

    CHECK_EQUAL(r, s);

    CHECK(ef != es);

    ef = es;

    CHECK(ef == es);
    CHECK_EQUAL(ef, es);

    Either<int, float> et(4.0f);
    CHECK(ef != et);
    CHECK_NOT_EQUAL(ef, et);

    bool sensed = false;
    ef.match<void>(
        [](int& i) { },
        [&sensed](const float& f) { sensed = true;});

    CHECK(sensed);
}

void test_nonpod() {
    Either<std::string, float> hootie("Hootie");
    Either<std::string, float> booper("booper");
    Either<std::string, float> sooper(3.14f);

    CHECK_EQUAL(hootie, hootie);
    CHECK_NOT_EQUAL(hootie, booper);
    CHECK_NOT_EQUAL(booper, sooper);
}

struct County {
    County()
        : count(1)
    { }

    County(const County& rhs)
        : count(rhs.count + 1)
    { }

    int count;
    static int destructy;

    ~County() {
        ++destructy;
    }
};

int County::destructy = 0;

void test_ctors() {
    County::destructy = 0;
    int copycount;
    {
        Either<County, std::string> a("Hody2");
        Either<County, std::string> b((County()));
        copycount = b.match<int>(
            [](const County& c) { return c.count; },
            [](const std::string& s) { return -1; });
    }

    CHECK_EQUAL(2, copycount);
    CHECK_EQUAL(2, County::destructy);

    Either<County, std::string> a("BLEEEEEH");
    Either<County, std::string> b("HUUUUUU");

    std::string result = a.match<std::string>(
        [](County) { return "wat"; },
        [&](const std::string& s1) {
            return b.match<std::string>(
                [](County c) { return "wat"; },
                [&](const std::string& s2) {
                    return s1 + "|" + s2;
            });
    });
}

void test_move1() {
    typedef Either<std::unique_ptr<int>, std::unique_ptr<float> > E;

    E e(std::unique_ptr<int>(new int(22)));
    E f = std::move(e);

    const auto& ep = left(e);
    const auto& fp = left(f);

    CHECK_EQUAL(nullptr, ep);
    CHECK_NOT_EQUAL(nullptr, fp);
    CHECK_EQUAL(22, *fp);
}

void test_right_function() {
    typedef Either<std::unique_ptr<int>, std::unique_ptr<float> > E;

    E g(std::unique_ptr<float>(new float(3.14)));
    E h = std::move(g);

    const auto& hp = right(h);
    CHECK_EQUAL(nullptr, right(g));
    CHECK_NOT_EQUAL(nullptr, hp);

    float f = *right(h);
    CHECK_ALMOST_EQUAL(3.14, f);
}

void test_move2() {
    typedef Either<std::unique_ptr<int>, std::unique_ptr<float> > E;

    E e(std::unique_ptr<int>(new int(22)));
    E f = std::move(e);

    E g(std::unique_ptr<float>(new float(3.14)));
    E h = std::move(g);

    h = std::move(f);

    CHECK_EQUAL(e, f);

    h = std::move(h);

    CHECK_EQUAL(22, *left(h));
}

void test_self_assignment() {
    typedef Either<std::shared_ptr<int>, float> E;

    E e(std::shared_ptr<int>(new int(999)));

    e = e;

    CHECK_NOT_EQUAL(nullptr, left(e));
    CHECK_EQUAL(999, *left(e));
}

void test_nonconst_function_accessors() {
    auto e = Either<int, float>::left(5);
    left(e)++;
    CHECK_EQUAL(6, left(e));

    auto f = Either<int, float>::right(2.1211f);
    right(f)++;
    CHECK_ALMOST_EQUAL(3.1211, right(f));
}

void test_either_with_same_types() {
    auto a = Either<int, int>::left(5);
    auto b = Either<int, int>::right(5);

    CHECK_NOT_EQUAL(a, b);

    CHECK_EQUAL(unsafeLeft(a), unsafeRight(b));
}

int main() {
    test_intfloat();
    test_nonpod();
    test_ctors();
    test_move1();
    test_right_function();
    test_move2();
    test_self_assignment();
    test_nonconst_function_accessors();
    test_either_with_same_types();
    return 0;
}
