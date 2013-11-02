#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "either.h"

void test(bool b, int line) {
    if (b) {
        printf("OK\n");
    } else {
        printf("FAIL on line %i\n", line);
        exit(1);
    }
}

#define CHECK(b) test(b, __LINE__)
#define CHECK_EQUAL(a, b) test((a) == (b), __LINE__)
#define CHECK_NOT_EQUAL(a, b) test((a) != (b), __LINE__)

int main() {
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

    return 0;
}
