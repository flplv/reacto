# Quality Firmware

[![Build status](https://gitlab.com/felipe-lavratti/qf/badges/master/build.svg)](https://gitlab.com/felipe-lavratti/qf/pipelines)
[![CE coverage report](https://gitlab.com/felipe-lavratti/qf/badges/master/coverage.svg)]()

Quality Firmware is a teaching class about quality practices on Embedded System software development. This project is the code base used in class.

Here you will find *demo and templates* for **Test Driven Development** (TDD) and **Unit Testing with Hardware**; a powerful, yet simple, **build system** that create test and application executables for development (your computer) and target (the hardware) platforms with **continuous integration** and **code coverage**; how to do **Object-Based Programming (OBP) in C**, and sometimes OOP; most common **design patterns** for embedded systems; a hand full of **reusable** code; and finally **a few big examples** showing how to apply OBP and use design patterns whiling following the cycles of TDD.

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

Inside the project root, this line will build the test executable `qf_demo_dev_tests` in the folder `./qf_demo_tests/build`:

```sh
    scons -Q ./qf_demo_tests/build/qf_demo_dev_tests
```
(You need to pass the full executable path to Scons to only build that single executable, otherwise it will try to build extra executable for target platform, which will require the LPCXpreesso tools installed.)

You can run tests with:

```sh
    ./qf_demo_tests/build/qf_demo_dev_tests
```

** 3. Generate and open code coverage **

To generate code coverage and open in firefox:
```sh
    scons -Q coverage
    firefox ./coverage/index.html
```

**If you want to build for the targeg LPCXpresso board**, download the IDE from the NXP web site and install on the default directory of your Ubuntu machine. This shall be enough to run the complete build with `scons -Q tests`.

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
    │   │   │   CppUTest is the test framework we are using, build scripts
    │   │   │    will download and build it here. CppuTest's libraries are
    │   │   │    created for each platform that tests will run.
    │   │   │   All projects built for tests include headers and libraries from
    │   │   │    here.
    │   │   │
    │   │   ├── *include/generated/CppUTestGeneratedConfig.h*
    │   │   │   This file configures the CppUTest build, it is shared between
    │   │   │    all platform builds.
    │   │   │
    │   │   └── *setup-cpputest.sh*
    │   │       This is the script responsible to download and decompress
    │   │        CppUTest source.
    │   │
    │   └── **lpc1769_board**
    │           I am using a LPCXpresso1769 development board, these are the
    │            dependent projects required to generate a complete firmware to
    │            the board.
    │           Every lpc1769 build will include headers and libraries here.
    │
    ├── **qf_demo**
    │   │       Finally the application code. No tests allowed here, nor
    │   │        platform dependent code, only application. Every dependency to
    │   │        external world must be abstracted in order to have a testable
    │   │        project.
    │   │       This project will generate a library that will be linked against
    │   │        tests builds and final builds.
    │   │
    │   └── *src*
    │       │   Here goes the source code of the application.
    │       │   This is an example project where you will find demo code
    │       │    and explanations about Quality Firmware Principles.
    │       │
    │       ├── *patterns*
    │       │   Empty design patterns goes here, explanations are in the
    │       │    headers.
    │       │
    │       ├── *reusables*
    │       │   Every project has reusable modules, such as checks, logs,
    │       │    FIFOs, etc. This is no different.
    │       │
    │       └── *watchdog*
    │           This is an extended example of a Software Watchdog Class created
    │            to provide diversified and safe watchdogs along the firwmare.
    │           A hardware watchdog will reboot by the expiration of any soft
    │            one.
    │
    ├── **qf_demo_tests**
    │   │       Finally our application tests. This project will create
    │   │        executables for each platform where tests will run.
    │   │       There are test code supposed to run on a platform but not on the
    │   │        others, this is why you have a source folder for each platform
    │   │        and one that will have tests that run on every platforms.
    │   │       Each sub folder of *qf_demo_tests* is equivalent of the *src*
    │   │        folder of the application. It means that, if you are testing a
    │   │        pattern, the test code will be at *platform_X/patterns/*.
    │   │
    │   ├── *platform_dev_tests*
    │   │       Test code that will run only on development platform, your
    │   │        computer.
    │   │
    │   ├── *platform_lpc176x_tests*
    │   │       Test code that will run only on the LPCXpresso board.
    │   │
    │   └── *tests*
    │           Test code that will run only on every platform.
    │
    └── *README.md*
                This README :)
```

## The build system explained

*To be described*

## Gitlab Continuous Integration and Coverage Report Page

*To be described*
