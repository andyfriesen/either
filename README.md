either
======

Haskell (and other functional languages) have a delightful feature called discriminated uninions.  They're like C unions, except that they have a tag denoting which 'branch' of the union is the right one, and syntax to ensure that you can never muck things up.

One of the most basic things you can get out of this is a type called Either.

Either<L, R> contains exactly one of either type L or type R.

This is handy for functions that can fail.  eg

```C++
Either<JsonError, string> r = extractJsonValue(someJsonThing, "key_name");
```
