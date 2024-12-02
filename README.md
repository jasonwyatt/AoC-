# AoC++: Advent of Code in C++

This is mostly for my own C++ learning purposes. I am not \[yet] a C++ expert, so beware.

## Building & Running

Example of building a given day:

```
bazel build //aoc2024:day01
```

And running the resulting binary:

```
bazel-bin/aoc2024/day01 aoc2024/data/day01.txt
```

### Building and running in one go:

```
bazel run //aoc2024:day01 -- $PWD/aoc2024/data/day01.txt
```