[![Travis](https://img.shields.io/travis/flplv/reacto.svg?style=flat-square)](https://travis-ci.org/flplv/reacto)
[![Codecov](https://img.shields.io/codecov/c/github/flplv/reacto.svg?style=flat-square)](https://codecov.io/gh/flplv/reacto)
[![GitHub release](https://img.shields.io/github/release/flplv/reacto.svg?style=flat-square)](https://github.com/flplv/reacto/releases)

# react.o
Portable framework for developing reactive embedded systems in C
- Project web page: [http://flplv.github.io/reacto/](http://flplv.github.io/reacto/)

# Change Log
[https://github.com/flplv/reacto/releases](https://github.com/flplv/reacto/releases)

# Project Planning
[https://gist.github.com/flplv/9043817e9ead01e7b13439d6060e5d69](https://gist.github.com/flplv/9043817e9ead01e7b13439d6060e5d69)

# Table of Content
<!-- TOC depthFrom:1 depthTo:6 withLinks:1 updateOnSave:1 orderedList:0 -->

- [react.o](#reacto)
- [Change Log](#change-log)
- [Table of Content](#table-of-content)
- [Project Structure](#project-structure)
- [How to Build](#how-to-build)
- [The build system](#the-build-system)
	- [Cross Building](#cross-building)
		- [Method 1: Copying code to the embedded project](#method-1-copying-code-to-the-embedded-project)
		- [Method 2: Add support to your platform in **react.o** build scripts](#method-2-add-support-to-your-platform-in-reacto-build-scripts)
- [Continuous Integration and Coverage Report Page](#continuous-integration-and-coverage-report-page)

<!-- /TOC -->

# Project Structure

```md
    .
    ├── **build_platforms**
    │           In this folder you will find the platforms definitions in
    │            Python/SCons. Each platform extends the class
    │            `PlatformBaseClass`. To add a new platform support, this is
    │            the only place you have to edit.
    │
    ├── **dependencies**
    │   │       Only for dependency project, no application code here,
    │   │        projects in this folder will not be considered on coverage
    │   │        reports.
    │   │       All projects inside this folder will be built into static
    │   │        libraries for each related platform.
    │   │
    │   └── **cpputest**
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

# How to Build

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

**3. Generate and open code coverage**

To generate code coverage and open in firefox:
```sh
    scons -Q coverage
    firefox ./coverage/index.html
```

If you need to know what targets are available to build:

```sh
    scons -Q
```
# The build system

## Cross Building

There are many ways to cross build **react.o**, one way is copying the code
to your project, and the other is to add a new platform to **react.o**'s build system.

### Method 1: Copying code to the embedded project

1. Copy folder `reacto/include` and `recto/src` to your project
2. Add the `include` folder to the compiler search path (`-I include`)
3. Now enable the GNU extensions with the gcc (or equivalent) flag `-std=gnu11`

You should now be able to build the your application successfully.

### Method 2: Add support to your platform in **react.o** build scripts

**react.o** uses Scons, a Python based build system, and created an expansible
build system where you can add one more platform considerably easy.

Platforms are defined in the folder `build_platforms`,
there you can find a few platform configured:

- `msp430.sconscript` defines a platform for the MSP430 chip, it is used to build the examples.
- `host_tests.sconscript` is a platform used during development, it build and run all **react.o**'s tests.

To create another platform, it is recommended to copy one of the available and setup:

- The python class name;
- The platform name returned in the `Name()` method;
- Define your own targets and return it in `TargetNameList()`;
- Set what projects will be built with your platform in the list returned by `BuildList()`;
- Choose `ProfileEnabled` and `CppUTestExtensionsEnabled` status;
- Create your `PostBuildTargets()` Commands
- Clone and setup your enviroment in `_SetupEnv()`, and finally;
- Use the new class when creating an instance at the end of the file.

# Continuous Integration and Coverage Report Page

*To be described*
