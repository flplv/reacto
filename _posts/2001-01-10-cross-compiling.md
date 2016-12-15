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
	- [Method 2: Changing **react.o** scripts to add support for another platform](#method-2-changing-reacto-scripts-to-add-support-for-another-platform)
		- [Defining a new platform](#defining-a-new-platform)
		- [Selecting the targets where the new platform will be used](#selecting-the-targets-where-the-new-platform-will-be-used)
		- [Running the build](#running-the-build)

<!-- /TOC -->

# Cross Building

There are many ways to cross build **react.o**, the easy way is copying the code
to your project, the harder is to configure **react.o**'s build system to support
your tool-chain.

## Method 1: Copying code to the embedded project

1. Copy folder `reacto/include` and `recto/src` to your project
2. Add the `include` folder to the compiler search path (`-I include`)
3. Now enable the GNU extensions with the gcc (or equivalent) flag `-std=gnu11`

You should now be able to build the your application successfully.

## Method 2: Changing **react.o** scripts to add support for another platform

**react.o** uses Scons, a Python based build system. You can add one more
platform in the build script. In this example we will create a build for the
LPC1769 chip, be aware you will have to replace "lpc176x" name with the name
you want to give.

### Defining a new platform

We start adding the new tool-chain path and the binaries prefix to variables to
the file `Sconscript` at the root of the repository.

```python
lpc176x_toolchain_path = '/usr/local/lpcxpresso_8.2.2_650/lpcxpresso/tools/bin'
lpc176x_toolchain_prefix = 'arm-none-eabi-'
```

Then we create the platform dictionary, where we setup:

    - Tool-chain binaries: `CC`, `CXX`, `LD`, etc;
    - Compiler defines with `CPPDEFINES`;
    - Compiler and linker flags with `CXXFLAGS`, `CCFLAGS`, `CFLAGS` and `LINKFLAGS`;

Add the code below to `Sconscript` and change it to meet your platform needs.
Leave `-std=gnu11` flag, it is required to build and use **react.o**.

```python
lpc176x_plat = {
    # Won't be used, we will not build CppUTest with this platform
    'cpputest_extensions_enabled' : True,

    # The build Environment definitions
    'env' : default_env.Clone (
        CC     = lpc176x_toolchain_prefix + 'gcc',
        CXX    = lpc176x_toolchain_prefix + "g++",
        LD     = lpc176x_toolchain_prefix + "g++",
        AR     = lpc176x_toolchain_prefix + "ar",
        STRIP  = lpc176x_toolchain_prefix + "strip",
        RANLIB = lpc176x_toolchain_prefix + "ranlib",

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
Add the line below to append tool-chain binary folder to the environment path:

```python
lpc176x_plat['env'].PrependENVPath('PATH', lpc176x_toolchain_path)
```

The platform is created and configured, now we must choose which projects
will use it.

### Selecting the targets where the new platform will be used

There's a dictionary called `platforms`, in the script, that is
exported to every project inside the build. We manipulate this variable, adding
and removing platforms to it, to
select what platforms will be used by subsequent calls to sub-folder `Sconscript`.
All sub-folder scripts are configured
to build one library or executable to each exported platform.

We are going to change this variable before calling each subscript.

For the platform we are adding we don't want to build CppUTest, it is not a test platform, but
a final one. We will only add our recently created platform to the `platforms`
variable after CppUTest's script is called by the line:

```python
 # Build CppUTest libraries for each platforms
cpputest = SConscript('dependencies/cpputest/Sconscript', exports ='platforms')
```

After the CppUTest SConscript function call
we add our platform to the `platforms` variable. Be aware that the resulting
library path depends on the dictionary key name used here:

```python
platforms['lpc176x'] = lpc176x_plat
```

Now, the next script that is called is to build **react.o** framework,
it will create a static library for each platform in `platforms` variable. The
line below is already in the script.

```python
 # Build reacto as library
SConscript('reacto/Sconscript', exports ='platforms')
```

After the **react.o** `Sconscript` being called, we will remove our platform
from `platforms` file to avoid it being used by the next projects. Add this line
after the code line above:

```python
del platforms['lpc176x']
```

We are almost finished. Scons uses its internal dependency discovery system to
select what is build and what is not, since this new platform library is not
a implicit dependency of any other target build in this project, Scons will not
build it by default.
We have to tell Scons to build the library explicitly, add the following line to
the script:

```python
Default('reacto/build/lpc176x/libreacto.a')
```

### Running the build

This is it, now run `scons` command from the terminal, it should build the
platform library in the path `reacto/build/lpc176x/libreacto.a`.

Link this library to your project, add **reac.o**'s include folder to your
project compiler flags and do not forget to enable GNU extensions.
