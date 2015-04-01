# Testing Wintermute {#testing}

Testing's an important part of Wintermute. It provides a level of insurance of
contract upholding between classes and higher-level interactions and allows for
rapid application development without too much concern for the breaking of
existing software. The test harness used by Wintermute is [CxxTest][] and
[CTest][] is used to drive the whole test suite (via `make test`). Coverage
support is handled by `gcov` and memory checking is handled by `valgrind`.

## Recommended Testing Targets

Thanks to `ctest`, testing can include coverage as well as memory leak
detection. After obtaining your sources and configuring the project for the
first time, you can set everything up for the first time by invoking

```
$ make Experimental
```

Of course, if you want to build a specific target, you can do so, but for
full project testing, this serves as a meta target to handle testing and the
other class of tests for the project.

## Wintermute's Fixture Library {#testing-fixture-lib}

Wintermute has a fixture library `wintermute-fixtures` that
allows test developers to write more introspective actions into Wintermute and
even take advantage of mock classes into Wintermute's operations that are
typically, by design, closed off. In essence, the fixture library is a
recompilation of the core Wintermute library with these additives, thus
preventing the parallel installation of said libraries.

### Available Fixtures {#testing-fixture-lib-fixtures}

### Available Macros {#testing-fixture-lib-macros}

### Available Functions {#testing-fixture-lib-functions}
