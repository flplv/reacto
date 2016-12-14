[![Build Status](https://travis-ci.org/felipe-lavratti/reacto.svg?branch=master)](https://travis-ci.org/felipe-lavratti/reacto)
[![Coverage Status](https://coveralls.io/repos/github/felipe-lavratti/reacto/badge.svg?branch=master)](https://coveralls.io/github/felipe-lavratti/reacto?branch=master)

# react.o

*(It is a working in progress)*

## How to Build

**1. Install Dependencies**

- GCC (The compiler)
- scons (The build generator used)
- wget (To download dependency source code)
- lcov (To generate code coverage reports)

In ubuntu:
```sh
    sudo apt-get install build-essential scons wget lcov
```

**2. Build and run Tests**

Inside the project root, this line will build the test executable `reacto_host_tests` in the folder `./reacto_tests/build`:

```sh
    scons -Q ./reacto_tests/build/reacto_host_tests
```

You can run tests with:

```sh
    ./reacto_tests/build/reacto_host_tests
```

** 3. Generate and open code coverage **

To generate code coverage and open in firefox:
```sh
    scons -Q coverage
    firefox ./coverage/index.html
```

## Project Structure

```md
    .
    ├── **dependencies**
    │   │       Only for dependency project, no application code here,
    │   │        projects in this folder will not be considered on coverage
    │   │        reports.
    │   │       All projects inside this folder will be built into static
    │   │        libraries for each related platform.
    │   │
    │   ├── **cpputest**
    │       │   CppUTest is the test framework we are using, build scripts
    │       │    will download and build it here. CppuTest's libraries are
    │       │    created for each platform that tests will run.
    │       │   All projects built for tests include headers and libraries from
    │       │    here.
    │       │
    │       ├── *include/generated/CppUTestGeneratedConfig.h*
    │       │   This file configures the CppUTest build, it is shared between
    │       │    all platform builds.
    │       │
    │       └── *setup-cpputest.sh*
    │           This is the script responsible to download and decompress
    │            CppUTest source.
    │
    ├── **reacto**
    │   │       Finally the application code. No tests allowed here, nor
    │   │        platform dependent code, only application. Every dependency to
    │   │        external world must be abstracted in order to have a testable
    │   │        project.
    │   │       This project will generate a library that will be linked against
    │   │        tests builds and final builds.
    │   │
    │   ├── *src*
    │   │   │   Here goes the source code of the application.
    │   │   │
    │   │   ├── *reusables*
    │   │   │   Every project has reusable modules, such as checks, logs,
    │   │   │    FIFOs, etc. This is no different.
    │   │   │
    │   │   └── *watchdog*
    │   │       This is an extended example of a Software Watchdog Class created
    │   │        to provide diversified and safe watchdogs along the firwmare.
    │   │       A hardware watchdog will reboot by the expiration of any soft
    │   │        one.
    │   │
    │   └── *includes*
    │           The include tree, has the same folders as the source tree. Files
    │            are thought to be public, include this folder in your project.
    │
    ├── **reacto_tests**
    │   │       Finally our application tests. This project will create
    │   │        executables for each platform where tests will run.
    │   │       There are test code supposed to run on a platform but not on the
    │   │        others, this is why you have a source folder for each platform
    │   │        and one that will have tests that run on every platforms.
    │   │       Each sub folder of *reacto_tests* is equivalent of the *src*
    │   │        folder of the application. It means that, if you are testing a
    │   │        pattern, the test code will be at *platform_X/patterns/*.
    │   │
    │   ├── *platform_host_tests*
    │   │       Test code that will run only on development platform, your
    │   │        computer.
    │   │
    │   └── *tests*
    │           Test code that will run only on every platform.
    │
    └── *README.md*
                This README :)
```

## The build system explained

*To be described*

## Continuous Integration and Coverage Report Page

*To be described*
