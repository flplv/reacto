# Examples

## rot13_factory

This is a rot13 algorithm example using the Factory (reacto/factory.h) sub-system where the initialization and allocation of 
every object related to the Main Loop is abstracted. 
It is an example supposed to run in the host machine, where an executable will be created.

The example architecture implements a datapath where the stdin is read by periodic polling, using the Timed Queue deferred job scheduler. The handler then pushes the character read to the rot13 queue, where the rot13 handler will tranform the data and then
pushe to the output queue, where te subsequent handler will output the processed character to the stdout. 

A sleep handler is installed in the Main Loop to let the program sleep whenever there's nothing to process, avoiding a busy loop that will occupy 100% of the CPU, or energy, if it was running in an embedded system.

## msp430 examples

The examples starting with msp430 are meant to run in a launchpad board. These examples doesn't use the Factory sub-system, instead, ccore elements are initialized individually and manually. 

### Building and running MSP430 examples

1. You need to install the compiler:

```sh
        sudo apt-get install binutils-msp430 gcc-msp430 msp430-libc mspdebug
```

2. Change the target MCU on the mcp430.sconscript file, variable `mcu_flag` in `_SetupEnv` method of the class.

3. Connect the Launchpad board to your usb port, and run:

```sh
    scons -Q msp430_blink_deploy
```
