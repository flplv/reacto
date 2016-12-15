---
category: The Build System
path: '/examples/:id'
title: 'Cross Building'
type: 'tutorial'
section: 'docs'
---
{::options auto_ids="true" /}

<!-- TOC depthFrom:1 depthTo:6 withLinks:1 updateOnSave:1 orderedList:0 -->

- [Cross building](#cross-building)
	- [Method 1: Copying code to the embedded project](#method-1-copying-code-to-the-embedded-project)
	- [Method 2: Changing **react.o** scripts to build a static library](#method-2-changing-reacto-scripts-to-build-a-static-library)

<!-- /TOC -->

# Cross building

There are many ways to cross build **react.o**, you can build it into a library,
copy the code to your project or configure **react.o**'s build system to support
your tool-chain.

The easiest way is copying it to your project and setting build flags.

## Method 1: Copying code to the embedded project

1. Copy folder `reacto/include` and `recto/src` to your project
2. Add the `include` folder to the compiler search path (`-I include`)
3. Now enable the gnu extensions with the gcc (or equivalent) flag `-std=gnu11`

You should now be able to build the your application successfully.

## Method 2: Changing **react.o** scripts to build a static library

**react.o** is built using Scons, a python based build system, open `Sconstruct`
file and create a new platform. In this example we will create a build for the
LPC1769 chip.

First we define tool-chain paths:
```python
lpc17xx_toolchain_path = '/usr/local/lpcxpresso_8.2.2_650/lpcxpresso/tools/bin'
lpc17xx_toolchain_prefix = 'arm-none-eabi-'
```

Then we create the platform dictionary:

```python
lpc176x_plat = {
    # Won't be used, we will not build CppUTest with this platform
    'cpputest_extensions_enabled' : True,

    # The build Environment definitions
    'env' : default_env.Clone (
        CC     = lpc17xx_toolchain_prefix + 'gcc',
        CXX    = lpc17xx_toolchain_prefix + "g++",
        LD     = lpc17xx_toolchain_prefix + "g++",
        AR     = lpc17xx_toolchain_prefix + "ar",
        STRIP  = lpc17xx_toolchain_prefix + "strip",
        RANLIB = lpc17xx_toolchain_prefix + "ranlib",

        # Change all defines and flags too meet your platform requirements
        CPPDEFINES = [
                      '__CODE_RED',
                      '__NEWLIB__',
                      '__LPC17XX__',
                      'CORE_M3',
                      ('__USE_CMSIS', 'CMSISv2p00_LPC17xx'),
                      'CPP_USE_HEAP',
                      ],
        CXXFLAGS = ['-std=gnu++0x', '-fno-rtti', '-fno-exceptions'],
        CCFLAGS = ['-O3', '-Wall', '-fmessage-length=0',
                   '-fno-builtin', '-ffunction-sections', '-fdata-sections',
                   '-mcpu=cortex-m3', '-mthumb'],
        # std=gnu11 is a react.o's requirement
        CFLAGS = ['-std=gnu11'],
        CPPINCLUDES = [],
        LINKFLAGS = [
                     '-nostdlib',
                     '-Xlinker',
                     '--gc-sections',
                     '-Xlinker',
                     '-print-memory-usage',
                     '-mcpu=cortex-m3',
                     '-mthumb'
                     ]
    )
}
```
Here we add tool-chain to the environment path:

```python
lpc176x_plat['env'].PrependENVPath('PATH', lpc17xx_toolchain_path)
```

The platform is created and configured, now we must choose what projects
will use it.

There's a dictionary called `platforms` in the Sconstruct script, that is
exported to every project inside the build. We manipulate this variable to
select what platforms will be used by the projects. Each project is configured
to build one library or executable to each provided platform though this
variable.

We are going to change this variable before calling each subscript.

For this platform we don't want to build CppUTest, it is not a test build, but
a final one. We will only add our recently created platform to the `platforms`
variable after CppUTest's script is called by the line:

```python
 # Build CppUTest libraries for each platforms
cpputest = SConscript('dependencies/cpputest/Sconscript', exports ='platforms')
```

finally we add our platform to the variable. Be aware that the resulting
library path depends on the dictionary key set here:

```python
platforms['lpc176x'] = lpc176x_plat
```

Now, the next script that is already called is to build **react.o** framework,
it is already configured to build a static library.

```python
 # Build reacto as library
SConscript('reacto/Sconscript', exports ='platforms')
```

After the **react.o** `Sconscript` being called, we will remove our platform
from `platforms` file to avoid it being used by the next projects.

```python
del platforms['lpc176x']
```

We have to tell scons to build the library explicitly, otherwise it wont build
since the file is not a dependency of any other target, add the line:

```python
Default('reacto/build/lpc176x/libreacto.a')
```

This is it, now run scons command from the terminal, it should build the
platform library in the path `reacto/build/lpc176x/libreacto.a`.
