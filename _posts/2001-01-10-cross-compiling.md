---
category: The Build System
path: '/examples/:id'
title: 'Cross Building'
type: 'tutorial'
section: 'docs'
---
{::options auto_ids="true" /}

<!-- TOC depthFrom:1 depthTo:6 withLinks:1 updateOnSave:1 orderedList:0 -->

- [Cross Building](#cross-building)
	- [Method 1: Copying code to the embedded project](#method-1-copying-code-to-the-embedded-project)
	- [Method 2: Add support to your platform in **react.o** build scripts](#method-2-add-support-to-your-platform-in-reacto-build-scripts)

<!-- /TOC -->

# Cross Building

There are many ways to cross build **react.o**, one way is copying the code
to your project, and the other is to add a new platform to **react.o**'s build system.

## Method 1: Copying code to the embedded project

1. Copy folder `reacto/include` and `recto/src` to your project
2. Add the `include` folder to the compiler search path (`-I include`)
3. Now enable the GNU extensions with the gcc (or equivalent) flag `-std=gnu11`

You should now be able to build the your application successfully.

## Method 2: Add support to your platform in **react.o** build scripts

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
