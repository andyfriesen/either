#pragma once

inline void test(bool b, const char* name, const char* doublename, int line) {
    if (b) {
        const std::string dots(40 - strlen(name), '.');
        printf("OK: %s.%s.\t%s\n", name, dots.c_str(), doublename);
    } else {
        printf("FAIL in %s %s line %i\n", name, doublename, line);
        exit(1);
    }
}

#define CHECK(b)                test(b, __FUNCTION__, #b, __LINE__)
#define CHECK_EQUAL(a, b)       test((a) == (b), __FUNCTION__, #a " == " #b, __LINE__)
#define CHECK_NOT_EQUAL(a, b)   test((a) != (b), __FUNCTION__, #a " != " #b, __LINE__)
#define CHECK_ALMOST_EQUAL(a, b) test(0.01 > abs(b - a), __FUNCTION__, #a " ~= " #b, __LINE__)

