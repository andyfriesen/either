#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory>
#include <iostream>
#include "optional.h"
#include "TestFramework.h"

void test_it() {
    Optional<int> a;
    Optional<int> b(99);

    CHECK(!a.hasValue());
    CHECK(b.hasValue());
    CHECK_EQUAL(99, b.value());
}

struct Counted {
    static int instanceCount;

    Counted() {
        instanceCount++;
    }

    ~Counted() {
        instanceCount--;
    }
};

int Counted::instanceCount = 0;

void test_non_pod() {
    CHECK_EQUAL(0, Counted::instanceCount);
    {
        Optional<std::unique_ptr<Counted> > o(std::unique_ptr<Counted>(new Counted));
        CHECK_EQUAL(1, Counted::instanceCount);
    }
    CHECK_EQUAL(0, Counted::instanceCount);
}

void test_move() {
    CHECK_EQUAL(0, Counted::instanceCount);
    Optional<std::unique_ptr<Counted> > o(std::unique_ptr<Counted>(new Counted));
    CHECK_EQUAL(1, Counted::instanceCount);
    CHECK(o.hasValue());
    {
        auto p = std::move(o.value());
    }
    CHECK_EQUAL(0, Counted::instanceCount);
}

int main() {
    test_it();
    test_non_pod();

    test_move();
    return 0;
}