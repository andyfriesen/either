either
======

Haskell (and other functional languages) have a delightful feature called discriminated unions.  They're like C unions, except that they have a tag denoting which 'branch' of the union is the right one, and syntax to ensure that you can never muck things up.

One of the most basic things you can get out of this is a type called `Either`.

`Either<Left, Right>` contains exactly one of either type `Left` or type `Right`.

This is handy for functions that can fail.  eg

```C++
Either<JsonError, string> r = extractJsonValue(someJsonThing, "key_name");
```

API
===

`Either<A, B>` can be implicitly converted from an `A` or `B`.

```c++
Either<int, const char*> e(5);
Either<int, const char*> f("Hello!");
```

Non-POD types are ok.

```c++
Either<ErrorCode, std::string> result = extractJsonKey(jsonDocument, keyName);
```

The best, safest way to pull stuff out of an `Either` is with its `match` method.  This ensures, by construction, that you do not treat a `left` as a `right` or vice versa:

```c++
e.match<void>(
    [](int i)         { printf("int %i\n", i); },
    [](const char* s) { printf("string '%s'\n", s); });
```

FIXME: `match` currently requires that you provide the return type as an explicit template argument.

```c++
Either<float, double> pi(3.141); // implicit conversion makes this so exciting!
double roundedUp = pi.match<double>(
    [](float f)  { return f; },
    [](double d) { return d; });
```

A few utility functions are provided for basic things: 

```c++
bool isLeft(const Either&)
bool isRight(const Either&)

LeftType& left(Either&)
RightType& right(Either&)
const LeftType& left(const Either&)
const RightType& right(const Either&)
LeftType&& left(Either&)
RightType&& right(Either&)

LeftType& unsafeLeft(Either&)
RightType& unsafeRight(Either&)
const LeftType& unsafeLeft(const Either&)
const RightType& unsafeRight(const Either&)
LeftType&& unsafeLeft(Either&&)
RightType&& unsafeRight(Either&&)
```

`left()` calls `abort()` if it is called on an `Either` that
has a `right` value, and vice versa.

`unsafeLeft` and `unsafeRight` do not do any kind of verification.  Only use them if you are sure.

```c++
Either<int, string> e = ...;

if (isLeft(e)) {
    printf("%i\n", unsafeLeft(e));
} else {
    printf("%s\n", unsafeRight(e).c_str());
}
```

It's reasonable and ok to grab a reference or a const reference out of an `Either`:

```c++
Either<int, float> e = ...;
left(e)++;
```

Move semantics are supported:

```c++
Either<std::unique_ptr<Error>, std::unique_ptr<Result> > e = ...;
auto f = std::move(e);
```
