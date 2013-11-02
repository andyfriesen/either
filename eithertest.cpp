#include <stdio.h>
#include <stdlib.h>
#include <memory>
#include <iostream>
#include "either.h"

void test(bool b, const char* name, const char* doublename, int line) {
    if (b) {
        printf("OK: %s\n", doublename);
    } else {
        printf("FAIL in %s %s line %i\n", name, doublename, line);
        exit(1);
    }
}

#define CHECK(b)                test(b, __FUNCTION__, #b, __LINE__)
#define CHECK_EQUAL(a, b)       test((a) == (b), __FUNCTION__, #a " == " #b, __LINE__)
#define CHECK_NOT_EQUAL(a, b)   test((a) != (b), __FUNCTION__, #a " != " #b, __LINE__)

void test_intfloat() {
    Either<int, float> ef(5);
    Either<int, float> es(5.0f);

    CHECK(ef.match<bool>(
        [](int i)   { return true; },
        [](float f) { return false; }));

    auto double_ = [](const Either<int, float>& ef) -> double {
        return ef.match<double>(
            [](const int& i)   { return i * 2; },
            [](const float& f) { return f * 2; }
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
    Either<std::string, float> sooper(3.14);

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

void test_move2() {
    typedef Either<std::unique_ptr<int>, std::unique_ptr<float> > E;

    E e(std::unique_ptr<int>(new int(22)));
    E f = std::move(e);

    E g(std::unique_ptr<float>(new float(3.14)));
    E h = std::move(g);

    h = std::move(f);

    CHECK_EQUAL(e, f);

    h = std::move(h);

    int value = h.match<int>(
        [](const std::unique_ptr<int>& i) { return *i; },
        [](const std::unique_ptr<float>& f) { return 0xdeadbeef; });

    CHECK_EQUAL(value, 22);
}

int main() {
    test_intfloat();
    test_nonpod();
    test_ctors();
    test_move1();
    test_move2();
    return 0;
}
