#include <stdio.h>
#include <iostream>
#include "either.h"

void testIt(Either<int, float>& ef) {
    ef.match(
        [](int i) { printf("int %i\n", i); },
        [](float f) { printf("float %f\n", f); }
    );
}

float double_(const Either<int, float>& ef) {
    return ef.match<float>(
        [](const int& i) { return i * 2; },
        [](const float& f) { return f * 2; }
    );
}

int main() {
    Either<int, float> ef(5);
    Either<int, float> es(5.0f);

    testIt(ef);
    testIt(es);

    double r = double_(ef);
    double s = double_(es);

    printf("BLAH %f %f\n", r, s);

    printf("SNTH\n");
    return 0;
}
